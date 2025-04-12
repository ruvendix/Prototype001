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

	m_respawnTimer = TimerManager::I()->CreateTimer(5.0f, false, this, &GameRoom::RespawnMonsters);
}

bool GameRoom::Update(float deltaSeconds)
{
	if (m_currentMonsterCount < MAX_MONSTER_RESPAWN_COUNT)
	{
		if (m_respawnTimer.IsTimerOn() == false)
		{
			m_respawnTimer.Reset();
			m_respawnTimer.SetOn();
		}

		m_respawnTimer.Update(deltaSeconds);
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

void GameRoom::RespawnMonsters()
{
	for (int32 i = 0; i < MAX_MONSTER_RESPAWN_COUNT; ++i)
	{ 
		uint32 newEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
		ASSERT_LOG_RETURN(m_mapMonster.find(newEntityId) == m_mapMonster.cend());
		const GameMonsterPtr& spNewGameMonster = GameMonsterFactory::I()->CreateRandomMonster(newEntityId);
		AddGameEntity(spNewGameMonster, true);
		++m_currentMonsterCount;
	}
}

void GameRoom::EnterGameRoom(const RxGameSessionPtr& spGameSession)
{
	uint32 newEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
	ASSERT_LOG_RETURN(m_mapPlayer.find(newEntityId) == m_mapPlayer.cend());

	// ���� �÷��̾� ����
	GamePlayerPtr spMainPlayer = std::make_shared<GamePlayer>();
	spMainPlayer->SetGameSession(spGameSession);
	spGameSession->SetGamePlayer(spMainPlayer);

	// ���� �÷��̾�� �⺻ ���� �ֱ�
	Protocol::EntityInfo mainPlayerInfo;
	mainPlayerInfo.set_entity_id(newEntityId);
	mainPlayerInfo.set_entity_type(Protocol::EEntityType::Player);
	mainPlayerInfo.set_entity_look_at_dir(Protocol::EEntityLookAtDirection::Down);
	mainPlayerInfo.set_entity_state(Protocol::EEntityState::Idle);
	mainPlayerInfo.set_cell_pos_x(6);
	mainPlayerInfo.set_cell_pos_y(6);
	mainPlayerInfo.set_max_hp(30);
	mainPlayerInfo.set_hp(30);
	//mainPlayerInfo.set_max_hp(3000);
	//mainPlayerInfo.set_hp(3000);
	mainPlayerInfo.set_attack(5);
	mainPlayerInfo.set_defense(2);

	spMainPlayer->SetGameEntityInfo(mainPlayerInfo);

	// ������ Ŭ���̾�Ʈ���� ���� �÷��̾� ���� ����
	const RxSendBufferPtr& pLocalGamePlayerPacket = RxServerPacketHandler::I()->MakeCreateMainPlayerPacket(mainPlayerInfo);
	spGameSession->Send(pLocalGamePlayerPacket);

	// ���� ������ �ִ� ��� ������Ʈ ������ ������ Ŭ���̾�Ʈ���� ����
	SyncGameEntities(spGameSession);

	// ������ Ŭ���̾�Ʈ�� ��� (���� ������ ���� ���� ��� Ŭ���̾�Ʈ���� �˸�)
	AddGameEntity(spMainPlayer, true);
}

void GameRoom::LeaveGameRoom(const RxGameSessionPtr& spGameSession)
{
	if (spGameSession == nullptr)
	{
		return;
	}

	// ������Ʈ ������ Id�� �˾ƾ� �ϴµ� �̰� �÷��̾ �ش�!
	const GamePlayerPtr& spPlayer = spGameSession->GetGamePlayer();
	RemoveGameEntity(spPlayer);
}

void GameRoom::SyncGameEntities(const RxGameSessionPtr& spGameSession)
{
	Protocol::S_SyncEntitiesPacket syncEntitiesPacket;

	for (auto gamePlayerIter : m_mapPlayer)
	{
		Protocol::EntityInfo* pPlayerInfo = syncEntitiesPacket.add_players_info();
		*pPlayerInfo = gamePlayerIter.second->GetGameEntityInfo();
	}

	for (auto gameMonsterIter : m_mapMonster)
	{
		Protocol::MonsterInfo* pMonsterInfo = syncEntitiesPacket.add_monsters_info();
		gameMonsterIter.second->CopyGameMonsterInfo(pMonsterInfo);
	}

	const RxSendBufferPtr& spSendSyncEntitiesPacket = RxServerPacketHandler::I()->MakeSyncEntitiesPacket(syncEntitiesPacket);
	spGameSession->Send(spSendSyncEntitiesPacket);
}

void GameRoom::AddGameEntity(const GameEntityPtr& spEntity, bool bBroadcast)
{
	Protocol::S_SyncEntitiesPacket syncEntitiesPacket;
	const Protocol::EntityInfo& spEntityInfo = spEntity->GetGameEntityInfo();

	uint64 entityId = spEntityInfo.entity_id();
	switch (spEntityInfo.entity_type())
	{
	case Protocol::EEntityType::Player:
	{
		const GamePlayerPtr& spPlayer = std::dynamic_pointer_cast<GamePlayer>(spEntity);
		auto insertedIter = m_mapPlayer.insert(std::make_pair(entityId, spPlayer));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(ServerErrorHandler, EServerErrorCode::EntityInsertFailed);
			return;
		}
	}
	break;

	case Protocol::EEntityType::Monster:
	{
		const GameMonsterPtr& spMonster = std::dynamic_pointer_cast<GameMonster>(spEntity);
		auto insertedIter = m_mapMonster.insert(std::make_pair(entityId, spMonster));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(ServerErrorHandler, EServerErrorCode::EntityInsertFailed);
			return;
		}
	}
	break;

	case Protocol::EEntityType::Projectile:
	{
		const GameProjectilePtr& spProjectilePtr = std::dynamic_pointer_cast<GameProjectile>(spEntity);
		auto insertedIter = m_mapProjectile.insert(std::make_pair(entityId, spProjectilePtr));
		if (insertedIter.second == false)
		{
			DETAIL_ERROR_LOG(ServerErrorHandler, EServerErrorCode::EntityInsertFailed);
			return;
		}
	}
	break;

	default:
		return;
	}

	// ������ ������ ��� Ŭ���̾�Ʈ���� ������ Ŭ���̾�Ʈ ���� ����
	if (bBroadcast == true)
	{
		spEntity->AddToSyncEntitiesPacket(syncEntitiesPacket);

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

	const Protocol::EntityInfo& entityInfo = spEntity->GetGameEntityInfo();
	uint64 entityId = entityInfo.entity_id();

	switch (entityInfo.entity_type())
	{
	case Protocol::EEntityType::Player:
		m_mapPlayer.erase(entityId);
		break;

	case Protocol::EEntityType::Monster:
		m_mapMonster.erase(entityId);
		--m_currentMonsterCount;
		break;

	case Protocol::EEntityType::Projectile:
		m_mapProjectile.erase(entityId);
		break;

	default:
		break;
	}
}

void GameRoom::ParsingPacket_MoveEntityPacket(const Protocol::C_MoveEntityPacket& moveEntityPacket) const
{
	const Protocol::EntityInfo& entityInfo = moveEntityPacket.entity_info();
	const GameEntityPtr& spEntity = FindGameEntity(entityInfo);
	ASSERT_LOG(spEntity != nullptr);

	// �̵� ���������� ���⼭ ����üũ �ʿ�!
	spEntity->ApplyGameEntityMoveInfo(entityInfo);

	// ����� ����� ��� �������� ����
	const RxSendBufferPtr& spSyncGameEntityMovePacket = RxServerPacketHandler::I()->MakeMoveEntityPacket(entityInfo);
	RxGameSessionManager::I()->Broadcast(spSyncGameEntityMovePacket);
}

void GameRoom::ParsingPacket_ModifyEntityLookAtDirectionPacket(const Protocol::C_ModifyEntityLookAtDirectionPacket& modifyEntityLookAtDirectionPacket) const
{
	const Protocol::EntityInfo& entityInfo = modifyEntityLookAtDirectionPacket.entity_info();
	const GameEntityPtr& spEntity = FindGameEntity(entityInfo);
	ASSERT_LOG(spEntity != nullptr);
	spEntity->ApplyGameEntityLookAtDirection(entityInfo);

	// ����� ����� ��� �������� ����
	const RxSendBufferPtr& spSendModifyEntityLookAtDirectionPacket = RxServerPacketHandler::I()->MakeModifyEntityLookAtDirectionPacket(entityInfo);
	RxGameSessionManager::I()->Broadcast(spSendModifyEntityLookAtDirectionPacket);
}

void GameRoom::ParsingPacket_ModifyEntityStatePacket(const Protocol::C_ModifyEntityStatePacket& modifyEntityStatePacket) const
{
	const Protocol::EntityInfo& entityInfo = modifyEntityStatePacket.entity_info();
	GameEntityPtr spEntity = FindGameEntity(entityInfo);
	ASSERT_LOG(spEntity != nullptr);
	spEntity->ApplyGameEntityState(entityInfo.entity_state());

	// ����� ����� ��� �������� ����
	const RxSendBufferPtr& spSendModifyEntityStatePacket = RxServerPacketHandler::I()->MakeModifyEntityStatePacket(entityInfo);
	RxGameSessionManager::I()->Broadcast(spSendModifyEntityStatePacket);
}

void GameRoom::ParsingPacket_AttackToEntityPacket(const Protocol::C_AttckToEntityPacket& attckToEntityPacket) const
{
	const Protocol::EntityInfo& attackerInfo = attckToEntityPacket.attacker_info();
	GameEntityPtr spAttacker = FindGameEntity(attackerInfo);
	ASSERT_LOG(spAttacker != nullptr);

	const Protocol::EntityInfo& victimInfo = attckToEntityPacket.victim_info();
	GameEntityPtr spVictim = FindGameEntity(victimInfo);
	ASSERT_LOG(spVictim != nullptr);

	// �������� ���ط� ���
	int32 remainVictimHp = CalculateVictimDamage(spAttacker->GetGameEntityInfo(), spVictim->GetGameEntityInfo());
	spVictim->ApplyGameEntityHp(remainVictimHp);

	const RxSendBufferPtr& hitDamagePacket = RxServerPacketHandler::I()->MakeHitDamageToEntityPacket(spAttacker->GetGameEntityInfo(), spVictim->GetGameEntityInfo());
	RxGameSessionManager::I()->Broadcast(hitDamagePacket);

	bool bEntityDeath = (remainVictimHp <= 0);
	if (bEntityDeath == true) // ���
	{
		spVictim->ApplyGameEntityState(Protocol::EEntityState::Death);

		const RxSendBufferPtr& dieEntityPacket = RxServerPacketHandler::I()->MakeDieEntityPacket(spVictim->GetGameEntityInfo());
		RxGameSessionManager::I()->Broadcast(dieEntityPacket);

		GameRoom::I()->RemoveGameEntity(spVictim);
		DEFAULT_TRACE_LOG("��ü ���! (Victim: %p)", spVictim);
	}
}

void GameRoom::ParsingPacket_CreateProjectilePacket(const Protocol::C_CreateProjectilePacket& createProjectilePacket)
{
	const Protocol::ProjectileInfo& projectileInfo = createProjectilePacket.projectile_info();
	const Protocol::EntityInfo& srcEntityInfo = projectileInfo.entity_info();

	uint32 newEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
	ASSERT_LOG_RETURN(m_mapProjectile.find(newEntityId) == m_mapProjectile.cend());

	// ����ü �⺻ ���� �ֱ�
	Protocol::ProjectileInfo destProjectileInfo;
	destProjectileInfo.set_projectile_id(projectileInfo.projectile_id());
	Protocol::EntityInfo* pDestEntityInfo = destProjectileInfo.mutable_entity_info();
	pDestEntityInfo->set_entity_id(newEntityId);
	pDestEntityInfo->set_entity_type(Protocol::EEntityType::Projectile);
	pDestEntityInfo->set_entity_look_at_dir(srcEntityInfo.entity_look_at_dir());
	pDestEntityInfo->set_entity_state(Protocol::EEntityState::Idle);
	pDestEntityInfo->set_cell_pos_x(srcEntityInfo.cell_pos_x());
	pDestEntityInfo->set_cell_pos_y(srcEntityInfo.cell_pos_y());
	pDestEntityInfo->set_attack(5);

	// ����ü ����
	GameProjectilePtr spProjectile = std::make_shared<GameProjectile>();
	spProjectile->SetGameEntityInfo(*pDestEntityInfo);
	AddGameEntity(spProjectile, false);

	// ������ ����ü�� ��Ŷ���� ����
	const RxSendBufferPtr& spSendCreateProjectilePacket = RxServerPacketHandler::I()->MakeCreateProjectilePacket(destProjectileInfo);
	RxGameSessionManager::I()->Broadcast(spSendCreateProjectilePacket);
}

void GameRoom::ParsingPacket_DieEntityPacket(const Protocol::C_DieEntityPacket& dieEntityPacket)
{
	const Protocol::EntityInfo& victimInfo = dieEntityPacket.victim_info();
	const GameEntityPtr& spVictim = FindGameEntity(victimInfo);
	RemoveGameEntity(spVictim);

	// ������ ��ü�� ��Ŷ���� ����
	const RxSendBufferPtr& spSendDieEntityPacket = RxServerPacketHandler::I()->MakeDieEntityPacket(victimInfo);
	RxGameSessionManager::I()->Broadcast(spSendDieEntityPacket);
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

GameEntityPtr GameRoom::FindGameEntity(const Protocol::EntityInfo& entityInfo) const
{
	GameEntityPtr spEntity = nullptr;
	switch (entityInfo.entity_type())
	{
	case Protocol::EEntityType::Player:
		spEntity = FindGamePlayer(entityInfo.entity_id());
		break;

	case Protocol::EEntityType::Monster:
		spEntity = FindGameMonster(entityInfo.entity_id());
		break;

	case Protocol::EEntityType::Projectile:
		spEntity = FindGameProjectile(entityInfo.entity_id());
		break;
	}

	return spEntity;
}

GameEntityPtr GameRoom::FindGameEntity(const Position2d& targetCellPos) const
{
	// �÷��̾���� Ȯ��
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

GameProjectilePtr GameRoom::FindGameProjectile(uint64 entityId) const
{
	auto foundIter = m_mapProjectile.find(entityId);
	if (foundIter == m_mapProjectile.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

int32 GameRoom::CalculateVictimDamage(const Protocol::EntityInfo& attackerInfo, const Protocol::EntityInfo& victimInfo) const
{
	int32 remainVictimHp = ((victimInfo.hp() + victimInfo.defense()) - attackerInfo.attack());
	remainVictimHp = global::Clamp(remainVictimHp, 0, victimInfo.max_hp());
	return remainVictimHp;
}