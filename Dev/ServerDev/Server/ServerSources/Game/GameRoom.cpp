// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameRoom.h"

#include "EngineSources/Actor/Actor.h"
#include "EngineSources/Actor/WorldTileMapActor.h"

DEFINE_SINGLETON(GameRoom);
DEFINE_COMPILETIMER_COUNTER(GameEntityIdCounter);

void GameRoom::Startup()
{
	WorldContext::I()->SetCellSize(48);
	m_spWorldTileMapActor = std::make_shared<WorldTileMapActor>();
	m_spWorldTileMapActor->SetUseResources(false);
	m_spWorldTileMapActor->Startup();
}

void GameRoom::Cleanup()
{
	m_spWorldTileMapActor->Cleanup();
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
	Protocol::GameEntityInfo& refNewLocalGamePlayerInfo = spNewLocalGamePlayer->GetGameEntityInfo();
	refNewLocalGamePlayerInfo.set_entity_id(newGameEntityId);
	refNewLocalGamePlayerInfo.set_entitye_look_at_dir(Protocol::EGameEntityLookAtDir::Down);
	refNewLocalGamePlayerInfo.set_entity_state(Protocol::EGameEntityState::Idle);
	refNewLocalGamePlayerInfo.set_cell_pos_x(6);
	refNewLocalGamePlayerInfo.set_cell_pos_y(6);

	// 입장한 클라이언트에게 게임 플레이어 정보 전송
	const RxSendBufferPtr& pLocalGamePlayerPacket = RxServerPacketHandler::I()->MakeCreateLocalGamePlayerPacket(refNewLocalGamePlayerInfo);
	spGameSession->Send(pLocalGamePlayerPacket);

	// 현재 서버에 있는 모든 오브젝트 정보를 입장한 클라이언트에게 전송
	SyncGameEntities(spGameSession);

	// 입장한 클라이언트를 등록 (현재 서버에 접속 중인 모든 클라이언트에게 알림)
	AddGameEntity(spNewLocalGamePlayer);
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
		Protocol::GameEntityInfo* pGamePlayerInfo = syncGameEntitiesPacket.add_game_entities();
		*pGamePlayerInfo = gamePlayerIter.second->GetGameEntityInfo();
	}

	for (auto gameMonsterIter : m_mapGameMonster)
	{
		Protocol::GameEntityInfo* pGameMonsterInfo = syncGameEntitiesPacket.add_game_entities();
		*pGameMonsterInfo = gameMonsterIter.second->GetGameEntityInfo();
	}

	const RxSendBufferPtr& spSendSyncGameEntitiesPacket = RxServerPacketHandler::I()->MakeSyncGameEntitiesPacket(syncGameEntitiesPacket);
	spGameSession->Send(spSendSyncGameEntitiesPacket);
}

void GameRoom::AddGameEntity(const GameEntityPtr& spGameEntity)
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
			DETAIL_ERROR_LOG(EErrorCode::ServerGameEntityInsertFailed);
			return;
		}
	}
	break;

	case Protocol::EGameEntityType::Monster:
	{
		auto insertedIter = m_mapGameMonster.insert(std::make_pair(entityId, spGameEntity));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(EErrorCode::ServerGameEntityInsertFailed);
			return;
		}
	}
	break;

	default:
		return;
	}

#pragma region 서버에 접속한 모든 클라이언트에게 입장한 클라이언트 정보 전송
	Protocol::S_SyncGameEntities syncGameEntitiesPacket;
	Protocol::GameEntityInfo* pNewGamePlayerEntityInfo = syncGameEntitiesPacket.add_game_entities();
	*pNewGamePlayerEntityInfo = (spGameEntity->GetGameEntityInfo());

	const RxSendBufferPtr& spSendSyncGameEntities = RxServerPacketHandler::I()->MakeSyncGameEntitiesPacket(syncGameEntitiesPacket);
	RxGameSessionManager::I()->Broadcast(spSendSyncGameEntities);
#pragma endregion
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
	const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntityLookAtDir.game_player_info();
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

void GameRoom::ParsingPacket_SyncGamePlayerMove(const Protocol::C_SyncGamePlayerMove& syncGamePlayerMove)
{
	const Protocol::GameEntityInfo& gamePlayerInfo = syncGamePlayerMove.game_player_info();
	const GamePlayerPtr& spGamePlayer = FindGamePlayer(gamePlayerInfo.entity_id());
	if (spGamePlayer == nullptr)
	{
		return;
	}

	// 이동 가능한지는 여기서 더블체크 필요!
	spGamePlayer->ApplyGameEntityMoveInfo(gamePlayerInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSyncGamePlayerMovePacket = RxServerPacketHandler::I()->MakeSyncGamePlayerMovePacket(gamePlayerInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGamePlayerMovePacket);
}

void GameRoom::ParsingPacket_SyncGameEntityState(const Protocol::C_SyncGameEntityState& syncGameEntityState)
{
	const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntityState.game_player_info();
	GameEntityPtr spGameEntity = FindGameEntity(gameEntityInfo);
	ASSERT_LOG(spGameEntity != nullptr);

	spGameEntity->ApplyGameEntityState(gameEntityInfo);

	// 변경된 사실을 모든 유저에게 전달
	const RxSendBufferPtr& spSyncGameEntityStatePacket = RxServerPacketHandler::I()->MakeSyncGameEntityStatePacket(gameEntityInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGameEntityStatePacket);
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
		break;
	}

	return spGameEntity;
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