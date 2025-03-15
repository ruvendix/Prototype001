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

	// ���ο� ���� �÷��̾� ����
	GamePlayerPtr spNewLocalGamePlayer = std::make_shared<GamePlayer>();
	spNewLocalGamePlayer->SetGameSession(spGameSession);
	spGameSession->SetGamePlayer(spNewLocalGamePlayer);

	// ���ο� ���� �÷��̾�� �⺻ ���� �ֱ�
	Protocol::GameEntityInfo& refNewLocalGamePlayerInfo = spNewLocalGamePlayer->GetGameEntityInfo();
	refNewLocalGamePlayerInfo.set_entity_id(newGameEntityId);
	refNewLocalGamePlayerInfo.set_entitye_look_at_dir(Protocol::EGameEntityLookAtDir::Down);
	refNewLocalGamePlayerInfo.set_entity_state(Protocol::EGameEntityState::Idle);
	refNewLocalGamePlayerInfo.set_cell_pos_x(6);
	refNewLocalGamePlayerInfo.set_cell_pos_y(6);

	// ������ Ŭ���̾�Ʈ���� ���� �÷��̾� ���� ����
	const RxSendBufferPtr& pLocalGamePlayerPacket = RxServerPacketHandler::I()->MakeCreateLocalGamePlayerPacket(refNewLocalGamePlayerInfo);
	spGameSession->Send(pLocalGamePlayerPacket);

	// ���� ������ �ִ� ��� ������Ʈ ������ ������ Ŭ���̾�Ʈ���� ����
	SyncGameEntities(spGameSession);

	// ������ Ŭ���̾�Ʈ�� ��� (���� ������ ���� ���� ��� Ŭ���̾�Ʈ���� �˸�)
	AddGameEntity(spNewLocalGamePlayer);
}

void GameRoom::LeaveGameRoom(const RxGameSessionPtr& spGameSession)
{
	if (spGameSession == nullptr)
	{
		return;
	}

	// ������Ʈ ������ Id�� �˾ƾ� �ϴµ� �̰� �÷��̾ �ش�!
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

#pragma region ������ ������ ��� Ŭ���̾�Ʈ���� ������ Ŭ���̾�Ʈ ���� ����
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

	// ����� ����� ��� �������� ����
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

	// �̵� ���������� ���⼭ ����üũ �ʿ�!
	spGamePlayer->ApplyGameEntityMoveInfo(gamePlayerInfo);

	// ����� ����� ��� �������� ����
	const RxSendBufferPtr& spSyncGamePlayerMovePacket = RxServerPacketHandler::I()->MakeSyncGamePlayerMovePacket(gamePlayerInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGamePlayerMovePacket);
}

void GameRoom::ParsingPacket_SyncGameEntityState(const Protocol::C_SyncGameEntityState& syncGameEntityState)
{
	const Protocol::GameEntityInfo& gameEntityInfo = syncGameEntityState.game_player_info();
	GameEntityPtr spGameEntity = FindGameEntity(gameEntityInfo);
	ASSERT_LOG(spGameEntity != nullptr);

	spGameEntity->ApplyGameEntityState(gameEntityInfo);

	// ����� ����� ��� �������� ����
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