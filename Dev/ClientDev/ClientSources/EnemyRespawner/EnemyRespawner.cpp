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
	m_respawnWaitTimer = TimerManager::I()->CreateTimer(5.0f, true, this, &EnemyRespawner::OnRespawn);
}

bool EnemyRespawner::Update(float deltaSeconds)
{
	if (m_currentEnemyCount < m_maxEnemyCount)
	{
		m_respawnWaitTimer.Update(deltaSeconds);
	}

	return true;
}

void EnemyRespawner::Cleanup()
{

}

void EnemyRespawner::RespawnEnemies(Scene* pCurrentScene)
{
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);
	for (int32 i = m_currentEnemyCount; i < m_maxEnemyCount; ++i)
	{
		// 여기가 리스폰 규칙
		auto foundPrototypeEnemyActorIter = m_mapPrototypeEnemyActors.find(0);
		ASSERT_LOG(foundPrototypeEnemyActorIter != m_mapPrototypeEnemyActors.cend());
		const std::shared_ptr<CellActor>& spPrototypeEnemyActor = (foundPrototypeEnemyActorIter->second);
		ASSERT_LOG(spPrototypeEnemyActor != nullptr);

		// 구분용 이름
		std::string strNewEnemyActorName;
		MakeFormatString(strNewEnemyActorName, "%s_%d", spPrototypeEnemyActor->GetActorName().c_str(), g_enemyId);
		++g_enemyId;

		const std::shared_ptr<CellActor>& spNewEnemyActor = pCurrentScene->CreateCloneActorToScene(spPrototypeEnemyActor);
		spNewEnemyActor->SetActorName(strNewEnemyActorName);

#if 0
		const Position2d& randomCellPos{ 6, 10 };
		spNewEnemyActor->ApplyCellPosition(randomCellPos);
#else
		const Position2d& randomCellPos = spNewEnemyActor->ApplyRandomCellPosition();
		DEFAULT_TRACE_LOG("%s 리스폰 위치(%d, %d)", strNewEnemyActorName.c_str(), randomCellPos.x, randomCellPos.y);
#endif

		CellActorMoveComponent* pNewEnemyActorMoveComponent = spNewEnemyActor->FindComponent<CellActorMoveComponent>();
		if (pNewEnemyActorMoveComponent != nullptr)
		{
			pNewEnemyActorMoveComponent->SetDestinationCellPosition(randomCellPos);
		}
	}

	m_currentEnemyCount = m_maxEnemyCount;
}

void EnemyRespawner::RespawnEnemy(const Protocol::GameMonsterInfo& gameMonsterInfo, Scene* pCurrentScene)
{
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	// 여기가 리스폰 규칙
	auto foundPrototypeEnemyActorIter = m_mapPrototypeEnemyActors.find(gameMonsterInfo.monster_id());
	ASSERT_LOG(foundPrototypeEnemyActorIter != m_mapPrototypeEnemyActors.cend());
	const std::shared_ptr<CellActor>& spPrototypeEnemyActor = (foundPrototypeEnemyActorIter->second);
	ASSERT_LOG(spPrototypeEnemyActor != nullptr);

	const Protocol::GameEntityInfo& gameEntityInfo = gameMonsterInfo.game_entity_info();

	// 구분용 이름
	std::string strNewEnemyActorName;
	MakeFormatString(strNewEnemyActorName, "%s_%d", spPrototypeEnemyActor->GetActorName().c_str(), gameEntityInfo.entity_id());

	const std::shared_ptr<CellActor>& spNewEnemyActor = pCurrentScene->CreateCloneActorToScene(spPrototypeEnemyActor);
	spNewEnemyActor->SetActorName(strNewEnemyActorName);

	Position2d respawnCellPos = { static_cast<int32>(gameEntityInfo.cell_pos_x()), static_cast<int32>(gameEntityInfo.cell_pos_y()) };
	spNewEnemyActor->ApplyCellPosition(respawnCellPos);
	DEFAULT_TRACE_LOG("%s 리스폰 위치(%d, %d)", strNewEnemyActorName.c_str(), respawnCellPos.x, respawnCellPos.y);

	CellActorMoveComponent* pNewEnemyActorMoveComponent = spNewEnemyActor->FindComponent<CellActorMoveComponent>();
	if (pNewEnemyActorMoveComponent != nullptr)
	{
		pNewEnemyActorMoveComponent->SetDestinationCellPosition(respawnCellPos);
	}
}

void EnemyRespawner::DecreaseEnemyCount()
{
	m_currentEnemyCount = global::Clamp(m_currentEnemyCount - 1, 0, m_maxEnemyCount);
}

void EnemyRespawner::OnRespawn()
{
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	RespawnEnemies(pCurrentScene);
	DEFAULT_TRACE_LOG("적군들 리스폰 개시!");
}