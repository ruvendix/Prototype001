// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyRespawner.h"

DEFINE_COMPILETIMER_COUNTER(EnemeyActorIdCounter);

namespace
{
	int32 g_enemyId = 0;
}

EnemyRespawner::EnemyRespawner()
{

}

EnemyRespawner::~EnemyRespawner()
{

}

void EnemyRespawner::Startup()
{

}

bool EnemyRespawner::Update(float deltaSeconds)
{
	return true;
}

void EnemyRespawner::Cleanup()
{

}

void EnemyRespawner::RespawnEnemy(const Protocol::MonsterInfo& monsterInfo, Scene* pCurrentScene)
{
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	// 여기가 리스폰 규칙
	auto foundPrototypeEnemyActorIter = m_mapPrototypeEnemyActors.find(monsterInfo.monster_id());
	ASSERT_LOG(foundPrototypeEnemyActorIter != m_mapPrototypeEnemyActors.cend());
	const std::shared_ptr<EnemyMonsterActor>& spPrototypeEnemyActor = (foundPrototypeEnemyActorIter->second);
	ASSERT_LOG(spPrototypeEnemyActor != nullptr);

	const Protocol::EntityInfo& entityInfo = monsterInfo.monster_info();

	// 구분용 이름
	std::string strNewEnemyActorName;
	MakeFormatString(strNewEnemyActorName, "%s_%d", spPrototypeEnemyActor->GetActorName().c_str(), entityInfo.entity_id());

	const std::shared_ptr<EnemyMonsterActor>& spNewEnemyActor = pCurrentScene->CreateCloneActorToScene(spPrototypeEnemyActor);
	spNewEnemyActor->SetActorName(strNewEnemyActorName);
	spNewEnemyActor->InitializeActorStateTable();
	spNewEnemyActor->RegisterStateOnBidirectional();
	spNewEnemyActor->SyncFromServer_EntityInfo(entityInfo);
	spNewEnemyActor->ImmediatelyChangeState<PawnActorIdleState>();

	Position2d respawnCellPos = { static_cast<int32>(entityInfo.cell_pos_x()), static_cast<int32>(entityInfo.cell_pos_y()) };
	DEFAULT_TRACE_LOG("%s 리스폰 위치(%d, %d)", strNewEnemyActorName.c_str(), respawnCellPos.x, respawnCellPos.y);
}