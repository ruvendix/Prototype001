// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameRoom.h"

#include "EngineSources/Actor/Actor.h"
#include "EngineSources/Actor/WorldTileMapActor.h"
#include "Monster/GameMonsterFactory.h"

DEFINE_SINGLETON(GameRoom);
DEFINE_COMPILETIMER_COUNTER(GameEntityIdCounter);

const uint32 MAX_MONSTER_RESPAWN_COUNT = 20;

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
	for (auto iter : m_mapPlayer)
	{
		const GamePlayerPtr& spPlayer = iter.second;
		ASSERT_LOG(spPlayer != nullptr);
		spPlayer->Update(deltaSeconds);
	}

	for (auto iter : m_mapMonster)
	{
		const GameMonsterPtr& spMonster = iter.second;
		ASSERT_LOG(spMonster != nullptr);
		spMonster->Update(deltaSeconds);
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
		uint32 newEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
		ASSERT_LOG_RETURN(m_mapMonster.find(newEntityId) == m_mapMonster.cend());
		const GameMonsterPtr& spNewGameMonster = GameMonsterFactory::I()->CreateRandomMonster(newEntityId);
		AddGameEntity(spNewGameMonster, false);
	}
}

void GameRoom::EnterGameRoom(const RxGameSessionPtr& spGameSession)
{
	uint32 newEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
	ASSERT_LOG_RETURN(m_mapPlayer.find(newEntityId) == m_mapPlayer.cend());

	// 메인 플레이어 생성
	GamePlayerPtr spMainPlayer = std::make_shared<GamePlayer>();
	spMainPlayer->SetGameSession(spGameSession);
	spGameSession->SetNetworkPlayer(spMainPlayer);

	// 메인 플레이어에게 기본 정보 넣기
	Protocol::NetworkEntityInfo mainPlayerInfo;
	mainPlayerInfo.set_entity_id(newEntityId);
	mainPlayerInfo.set_entity_type(Protocol::ENetworkEntityType::Player);
	mainPlayerInfo.set_entitye_look_at_dir(Protocol::ENetworkEntityLookAtDirection::Down);
	mainPlayerInfo.set_entity_state(Protocol::ENetworkEntityState::Idle);
	mainPlayerInfo.set_cell_pos_x(6);
	mainPlayerInfo.set_cell_pos_y(6);
	mainPlayerInfo.set_max_hp(30);
	mainPlayerInfo.set_hp(30);
	mainPlayerInfo.set_defense(2);

	spMainPlayer->SetGameEntityInfo(mainPlayerInfo);

	// 입장한 클라이언트에게 게임 플레이어 정보 전송
	const RxSendBufferPtr& pLocalGamePlayerPacket = RxServerPacketHandler::I()->MakeCreateMainPlayerPacket(mainPlayerInfo);
	spGameSession->Send(pLocalGamePlayerPacket);

	// 현재 서버에 있는 모든 오브젝트 정보를 입장한 클라이언트에게 전송
	SyncGameEntities(spGameSession);

	// 입장한 클라이언트를 등록 (현재 서버에 접속 중인 모든 클라이언트에게 알림)
	AddGameEntity(spMainPlayer, true);
}

void GameRoom::LeaveGameRoom(const RxGameSessionPtr& spGameSession)
{
	if (spGameSession == nullptr)
	{
		return;
	}

	// 오브젝트 정보의 Id를 알아야 하는데 이건 플레이어만 해당!
	const GamePlayerPtr& spPlayer = spGameSession->GetGamePlayer();
	RemoveGameEntity(spPlayer);
}

void GameRoom::SyncGameEntities(const RxGameSessionPtr& spGameSession)
{
	Protocol::S_SyncEntitiesPacket syncEntitiesPacket;

	for (auto gamePlayerIter : m_mapPlayer)
	{
		Protocol::NetworkEntityInfo* pPlayerInfo = syncEntitiesPacket.add_players_info();
		*pPlayerInfo = gamePlayerIter.second->GetGameEntityInfo();
	}

	for (auto gameMonsterIter : m_mapMonster)
	{
		Protocol::NetworkMonsterInfo* pMonsterInfo = syncEntitiesPacket.add_monsters_info();
		gameMonsterIter.second->CopyGameMonsterInfo(pMonsterInfo);
	}

	const RxSendBufferPtr& spSendSyncEntitiesPacket = RxServerPacketHandler::I()->MakeSyncEntitiesPacket(syncEntitiesPacket);
	spGameSession->Send(spSendSyncEntitiesPacket);
}

void GameRoom::AddGameEntity(const GameEntityPtr& spEntity, bool bBroadcast)
{
	const Protocol::NetworkEntityInfo& spEntityInfo = spEntity->GetGameEntityInfo();

	uint64 entityId = spEntityInfo.entity_id();
	switch (spEntityInfo.entity_type())
	{
	case Protocol::ENetworkEntityType::Player:
	{
		const GamePlayerPtr& spPlayer = std::dynamic_pointer_cast<GamePlayer>(spEntity);
		auto insertedIter = m_mapPlayer.insert(std::make_pair(entityId, spPlayer));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(ServerErrorHandler, EServerErrorCode::NetworkEntityInsertFailed);
			return;
		}
	}
	break;

	case Protocol::ENetworkEntityType::Monster:
	{
		const GameMonsterPtr& spMonster = std::dynamic_pointer_cast<GameMonster>(spEntity);
		auto insertedIter = m_mapMonster.insert(std::make_pair(entityId, spMonster));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(ServerErrorHandler, EServerErrorCode::NetworkEntityInsertFailed);
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
		Protocol::S_SyncEntitiesPacket syncEntitiesPacket;
		Protocol::NetworkEntityInfo* pNewPlayerEntityInfo = syncEntitiesPacket.add_players_info();
		*pNewPlayerEntityInfo = (spEntity->GetGameEntityInfo());

		const RxSendBufferPtr& spSendSyncEntities = RxServerPacketHandler::I()->MakeSyncEntitiesPacket(syncEntitiesPacket);
		RxGameSessionManager::I()->Broadcast(spSendSyncEntities);
	}
}

void GameRoom::RemoveGameEntity(const GameEntityPtr& spEntity)
{
	if (spEntity == nullptr)
	{
		return;
	}

	const Protocol::NetworkEntityInfo& entityInfo = spEntity->GetGameEntityInfo();
	uint64 entityId = entityInfo.entity_id();

	switch (entityInfo.entity_type())
	{
	case Protocol::ENetworkEntityType::Player:
		m_mapPlayer.erase(entityId);
		break;

	case Protocol::ENetworkEntityType::Monster:
		m_mapMonster.erase(entityId);
		break;

	default:
		break;
	}
}

void GameRoom::ParsingPacket_MoveEntityPacket(const Protocol::C_MoveEntityPacket& moveEntityPacket) const
{
	const Protocol::NetworkEntityInfo& entityInfo = moveEntityPacket.entity_info();
	const GameEntityPtr& spEntity = FindGameEntity(entityInfo);
	ASSERT_LOG(spEntity != nullptr);

	// 이동 가능한지는 여기서 더블체크 필요!
	spEntity->ApplyGameEntityMoveInfo(entityInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSyncGameEntityMovePacket = RxServerPacketHandler::I()->MakeMoveEntityPacket(entityInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGameEntityMovePacket);
}

void GameRoom::ParsingPacket_ModifyEntityLookAtDirectionPacket(const Protocol::C_ModifyEntityLookAtDirectionPacket& modifyEntityLookAtDirectionPacket) const
{
	const Protocol::NetworkEntityInfo& entityInfo = modifyEntityLookAtDirectionPacket.entity_info();
	const GameEntityPtr& spEntity = FindGameEntity(entityInfo);
	ASSERT_LOG(spEntity != nullptr);
	spEntity->ApplyGameEntityLookAtDirection(entityInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSendModifyEntityLookAtDirectionPacket = RxServerPacketHandler::I()->MakeModifyEntityLookAtDirectionPacket(entityInfo);
	RxGameSessionManager::I()->Broadcast(spSendModifyEntityLookAtDirectionPacket);
}

void GameRoom::ParsingPacket_ModifyEntityStatePacket(const Protocol::C_ModifyEntityStatePacket& modifyEntityStatePacket) const
{
	const Protocol::NetworkEntityInfo& entityInfo = modifyEntityStatePacket.entity_info();
	GameEntityPtr spEntity = FindGameEntity(entityInfo);
	ASSERT_LOG(spEntity != nullptr);
	spEntity->ApplyGameEntityState(entityInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSendModifyEntityStatePacket = RxServerPacketHandler::I()->MakeModifyEntityStatePacket(entityInfo);
	RxGameSessionManager::I()->Broadcast(spSendModifyEntityStatePacket);
}

Position2d GameRoom::GenerateRandomCellPosition() const
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

bool GameRoom::CheckCanMoveToCellPosition(const Position2d& destCellPos, const GameEntityPtr& spExcludeEntity) const
{
	if (m_spWorldTileMapActor->CheckMovingAvailableTile(destCellPos) == false)
	{
		return false;
	}

	const GameEntityPtr& spFoundEntity = FindGameEntity(destCellPos);
	if ((spFoundEntity != nullptr) ||
		(spFoundEntity != spExcludeEntity))
	{
		return false;
	}

	return true;
}

GameEntityPtr GameRoom::FindGameEntity(const Protocol::NetworkEntityInfo& entityInfo) const
{
	GameEntityPtr spEntity = nullptr;
	switch (entityInfo.entity_type())
	{
	case Protocol::ENetworkEntityType::Player:
		spEntity = FindGamePlayer(entityInfo.entity_id());
		break;

	case Protocol::ENetworkEntityType::Monster:
		spEntity = FindGameMonster(entityInfo.entity_id());
		break;
	}

	return spEntity;
}

GameEntityPtr GameRoom::FindGameEntity(const Position2d& targetCellPos) const
{
	// 플레이어부터 확인
	for (auto foundIter : m_mapPlayer)
	{
		const GamePlayerPtr& spPlayer = foundIter.second;
		ASSERT_LOG(spPlayer != nullptr);
		if (spPlayer->CheckSameCellPosition(targetCellPos) == true)
		{
			return spPlayer;
		}
	}

	return nullptr;
}

GamePlayerPtr GameRoom::FindGamePlayer(uint64 entityId) const
{
	auto foundIter = m_mapPlayer.find(entityId);
	if (foundIter == m_mapPlayer.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

GameMonsterPtr GameRoom::FindGameMonster(uint64 entityId) const
{
	auto foundIter = m_mapMonster.find(entityId);
	if (foundIter == m_mapMonster.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}