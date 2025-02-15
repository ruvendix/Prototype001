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
		// ���Ⱑ ������ ��Ģ
		auto foundPrototypeEnemyActorIter = m_mapPrototypeEnemyActors.find(0);
		ASSERT_LOG(foundPrototypeEnemyActorIter != m_mapPrototypeEnemyActors.cend());
		const std::shared_ptr<CellActor>& spPrototypeEnemyActor = (foundPrototypeEnemyActorIter->second);
		ASSERT_LOG(spPrototypeEnemyActor != nullptr);

		// ���п� �̸�
		std::string strNewEnemyActorName;
		MakeFormatString(strNewEnemyActorName, "%s_%d", spPrototypeEnemyActor->GetActorName().c_str(), g_enemyId);
		++g_enemyId;

		const std::shared_ptr<CellActor>& spNewEnemyActor = pCurrentScene->CreateCloneActorToScene(spPrototypeEnemyActor);
#if 1
		const Position2d& randomCellPos{ 6, 10 };
		spNewEnemyActor->ApplyCellPosition(randomCellPos);
#else
		const Position2d& randomCellPos = spNewEnemyActor->ApplyRandomCellPosition();
		DEFAULT_TRACE_LOG("%s ������ ��ġ(%d, %d)", strNewEnemyActorName.c_str(), randomCellPos.x, randomCellPos.y);
#endif

		CellActorMoveComponent* pNewEnemyActorMoveComponent = spNewEnemyActor->FindComponent<CellActorMoveComponent>();
		if (pNewEnemyActorMoveComponent != nullptr)
		{
			pNewEnemyActorMoveComponent->SetDestinationCellPosition(randomCellPos);
		}
	}

	m_currentEnemyCount = m_maxEnemyCount;
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
	DEFAULT_TRACE_LOG("������ ������ ����!");
}