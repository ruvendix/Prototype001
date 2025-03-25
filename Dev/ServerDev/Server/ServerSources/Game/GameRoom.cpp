// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameRoom.h"

#include "EngineSources/Actor/Actor.h"
#include "EngineSources/Actor/WorldTileMapActor.h"
#include "Monster/GameMonsterFactory.h"

DEFINE_SINGLETON(GameRoom);
DEFINE_COMPILETIMER_COUNTER(GameEntityIdCounter);

const uint32 MAX_MONSTER_RESPAWN_COUNT = 1;

void GameRoom::Startup()
{
	WorldContext::I()->SetWorldSize(Size{3024, 2064});
	WorldContext::I()->SetCellSize(48);

	m_spWorldTileMapActor = std::make_shared<WorldTileMapActor>();
	m_spWorldTileMapActor->SetUseResources(false);
	m_spWorldTileMapActor->Startup();

	// 몬스터 정보 추가
	InitializeMonsters();
}

bool GameRoom::Update(float deltaSeconds)
{
	for (auto iter : m_mapGamePlayer)
	{
		const GamePlayerPtr& spGamePlayer = iter.second;
		ASSERT_LOG(spGamePlayer != nullptr);
		spGamePlayer->Update(deltaSeconds);
	}

	for (auto iter : m_mapGameMonster)
	{
		const GameMonsterPtr& spGameMonster = iter.second;
		ASSERT_LOG(spGameMonster != nullptr);
		spGameMonster->Update(deltaSeconds);
	}

	return true;
}

void GameRoom::Cleanup()
{
	m_spWorldTileMapActor->Cleanup();
}

void GameRoom::InitializeMonsters()
{
	for (int32 i = 0; i < MAX_MONSTER_RESPAWN_COUNT; ++i)
	{
		uint32 newGameEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
		ASSERT_LOG_RETURN(m_mapGameMonster.find(newGameEntityId) == m_mapGameMonster.cend());
		const GameMonsterPtr& spNewGameMonster = GameMonsterFactory::I()->CreateRandomMonster(newGameEntityId);
		AddGameEntity(spNewGameMonster, false);
	}
}

void GameRoom::EnterGameRoom(const RxGameSessionPtr& spGameSession)
{
	uint32 newGameEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
	ASSERT_LOG_RETURN(m_mapGamePlayer.find(newGameEntityId) == m_mapGamePlayer.cend());

	// 새로운 게임 플레이어 생성
	GamePlayerPtr spNewLocalGamePlayer = std::make_shared<GamePlayer>();
	spNewLocalGamePlayer->SetGameSession(spGameSession);
	spGameSession->SetGamePlayer(spNewLocalGamePlayer);

	// 새로운 게임 플레이어에게 기본 정보 넣기
	Protocol::GameEntityInfo newLocalGamePlayerInfo;
	newLocalGamePlayerInfo.set_entity_id(newGameEntityId);
	newLocalGamePlayerInfo.set_entity_type(Protocol::EGameEntityType::Player);
	newLocalGamePlayerInfo.set_entitye_look_at_dir(Protocol::EGameEntityLookAtDir::Down);
	newLocalGamePlayerInfo.set_entity_state(Protocol::EGameEntityState::Idle);
	newLocalGamePlayerInfo.set_cell_pos_x(6);
	newLocalGamePlayerInfo.set_cell_pos_y(6);

	spNewLocalGamePlayer->SetGameEntityInfo(newLocalGamePlayerInfo);

	// 입장한 클라이언트에게 게임 플레이어 정보 전송
	const RxSendBufferPtr& pLocalGamePlayerPacket = RxServerPacketHandler::I()->MakeCreateLocalGamePlayerPacket(newLocalGamePlayerInfo);
	spGameSession->Send(pLocalGamePlayerPacket);

	// 현재 서버에 있는 모든 오브젝트 정보를 입장한 클라이언트에게 전송
	SyncGameEntities(spGameSession);

	// 입장한 클라이언트를 등록 (현재 서버에 접속 중인 모든 클라이언트에게 알림)
	AddGameEntity(spNewLocalGamePlayer, true);
}

void GameRoom::LeaveGameRoom(const RxGameSessionPtr& spGameSession)
{
	if (spGameSession == nullptr)
	{
		return;
	}

	// 오브젝트 정보의 Id를 알아야 하는데 이건 플레이어만 해당!
	const GamePlayerPtr& spGamePlayer = spGameSession->GetGamePlayer();
	RemoveGameEntity(spGamePlayer);
}

void GameRoom::SyncGameEntities(const RxGameSessionPtr& spGameSession)
{
	Protocol::S_SyncGameEntities syncGameEntitiesPacket;

	for (auto gamePlayerIter : m_mapGamePlayer)
	{
		Protocol::GameEntityInfo* pGamePlayerInfo = syncGameEntitiesPacket.add_players_info();
		*pGamePlayerInfo = gamePlayerIter.second->GetGameEntityInfo();
	}

	for (auto gameMonsterIter : m_mapGameMonster)
	{
		Protocol::GameMonsterInfo* pGameMonsterInfo = syncGameEntitiesPacket.add_monsters_info();
		gameMonsterIter.second->CopyGameMonsterInfo(pGameMonsterInfo);
	}

	const RxSendBufferPtr& spSendSyncGameEntitiesPacket = RxServerPacketHandler::I()->MakeSyncGameEntitiesPacket(syncGameEntitiesPacket);
	spGameSession->Send(spSendSyncGameEntitiesPacket);
}

void GameRoom::AddGameEntity(const GameEntityPtr& spGameEntity, bool bBroadcast)
{
	const Protocol::GameEntityInfo& spGameEntityInfo = spGameEntity->GetGameEntityInfo();

	uint64 entityId = spGameEntityInfo.entity_id();
	switch (spGameEntityInfo.entity_type())
	{
	case Protocol::EGameEntityType::Player:
	{
		const GamePlayerPtr& spGamePlayer = std::dynamic_pointer_cast<GamePlayer>(spGameEntity);
		auto insertedIter = m_mapGamePlayer.insert(std::make_pair(entityId, spGamePlayer));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(ServerErrorHandler, EServerErrorCode::NetworkGameEntityInsertFailed);
			return;
		}
	}
	break;

	case Protocol::EGameEntityType::Monster:
	{
		const GameMonsterPtr& spGameMonster = std::dynamic_pointer_cast<GameMonster>(spGameEntity);
		auto insertedIter = m_mapGameMonster.insert(std::make_pair(entityId, spGameMonster));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(ServerErrorHandler, EServerErrorCode::NetworkGameEntityInsertFailed);
			return;
		}
	}
	break;

	default:
		return;
	}

	// 서버에 접속한 모든 클라이언트에게 입장한 클라이언트 정보 전송
	if (bBroadcast == true)
	{
		Protocol::S_SyncGameEntities syncGameEntitiesPacket;
		Protocol::GameEntityInfo* pNewGamePlayerEntityInfo = syncGameEntitiesPacket.add_players_info();
		*pNewGamePlayerEntityInfo = (spGameEntity->GetGameEntityInfo());

		const RxSendBufferPtr& spSendSyncGameEntities = RxServerPacketHandler::I()->MakeSyncGameEntitiesPacket(syncGameEntitiesPacket);
		RxGameSessionManager::I()->Broadcast(spSendSyncGameEntities);
	}
}

void GameRoom::RemoveGameEntity(const GameEntityPtr& spGameEntity)
{
	if (spGameEntity == nullptr)
	{
		return;
	}

	const Protocol::GameEntityInfo& gameEntityInfo = spGameEntity->GetGameEntityInfo();
	uint64 gameEntityId = gameEntityInfo.entity_id();

	switch (gameEntityInfo.entity_type())
	{
	case Protocol::EGameEntityType::Player:
		m_mapGamePlayer.erase(gameEntityId);
		break;

	case Protocol::EGameEntityType::Monster:
		m_mapGameMonster.erase(gameEntityId);
		break;

	default:
		break;
	}
}

void GameRoom::ParsingPacket_SyncGameEntityLookAtDirection(const Protocol::C_SyncGameEntityLookAtDir& syncGameEntityLookAtDir)
{
	const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntityLookAtDir.entity_info();
	const GamePlayerPtr& spGameEntity = FindGamePlayer(gameEntityInfo.entity_id());
	if (spGameEntity == nullptr)
	{
		return;
	}

	spGameEntity->ApplyGameEntityLookAtDirection(gameEntityInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSyncGameEntityLookAtDirPacket = RxServerPacketHandler::I()->MakeSyncGameEntityLookAtDirectionPacket(gameEntityInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGameEntityLookAtDirPacket);
}

void GameRoom::ParsingPacket_SyncGameEntityMove(const Protocol::C_SyncGameEntityMove& syncGameEntityMove)
{
	const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntityMove.entity_info();
	const GameEntityPtr& spGameEntity = FindGameEntity(gameEntityInfo);
	if (spGameEntity == nullptr)
	{
		return;
	}

	// 이동 가능한지는 여기서 더블체크 필요!
	spGameEntity->ApplyGameEntityMoveInfo(gameEntityInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSyncGameEntityMovePacket = RxServerPacketHandler::I()->MakeSyncGameEntityMovePacket(gameEntityInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGameEntityMovePacket);
}

void GameRoom::ParsingPacket_SyncGameEntityState(const Protocol::C_SyncGameEntityState& syncGameEntityState)
{
	const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntityState.entity_info();
	GameEntityPtr spGameEntity = FindGameEntity(gameEntityInfo);
	ASSERT_LOG(spGameEntity != nullptr);

	spGameEntity->ApplyGameEntityState(gameEntityInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSyncGameEntityStatePacket = RxServerPacketHandler::I()->MakeSyncGameEntityStatePacket(gameEntityInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGameEntityStatePacket);
}

Position2d GameRoom::GenerateRandomCellPosition()
{
	Position2d randomCellPos;

	while (true)
	{
		const Position2d& tempRandomCellPos = WorldContext::I()->CalculateRandomCellPosition();
		if (CheckCanMoveToCellPosition(tempRandomCellPos, nullptr) == true)
		{
			randomCellPos = tempRandomCellPos;
			break;
		}
	}

	return randomCellPos;
}

bool GameRoom::CheckCanMoveToCellPosition(const Position2d& destCellPos, const GameEntityPtr& spExcludeGameEntity) const
{
	if (m_spWorldTileMapActor->CheckMovingAvailableTile(destCellPos) == false)
	{
		return false;
	}

	const GameEntityPtr& spFoundGameEntity = FindGameEntity(destCellPos);
	if ((spFoundGameEntity != nullptr) ||
		(spFoundGameEntity != spExcludeGameEntity))
	{
		return false;
	}

	return true;
}

GameEntityPtr GameRoom::FindGameEntity(const Protocol::GameEntityInfo& gameEntityInfo) const
{
	GameEntityPtr spGameEntity = nullptr;
	switch (gameEntityInfo.entity_type())
	{
	case Protocol::EGameEntityType::Player:
		spGameEntity = FindGamePlayer(gameEntityInfo.entity_id());
		break;

	case Protocol::EGameEntityType::Monster:
		spGameEntity = FindGameMonster(gameEntityInfo.entity_id());
		break;
	}

	return spGameEntity;
}

GameEntityPtr GameRoom::FindGameEntity(const Position2d& targetCellPos) const
{
	// 플레이어부터 확인
	for (auto foundIter : m_mapGamePlayer)
	{
		const GamePlayerPtr& spGamePlayer = foundIter.second;
		ASSERT_LOG(spGamePlayer != nullptr);
		if (spGamePlayer->CheckSameCellPosition(targetCellPos) == true)
		{
			return spGamePlayer;
		}
	}

	return nullptr;
}

GamePlayerPtr GameRoom::FindGamePlayer(uint64 entityId) const
{
	auto foundIter = m_mapGamePlayer.find(entityId);
	if (foundIter == m_mapGamePlayer.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

GameMonsterPtr GameRoom::FindGameMonster(uint64 entityId) const
{
	auto foundIter = m_mapGameMonster.find(entityId);
	if (foundIter == m_mapGameMonster.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}