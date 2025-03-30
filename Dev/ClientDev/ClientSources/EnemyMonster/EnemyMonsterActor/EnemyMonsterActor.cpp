// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterActor.h"

#include "EngineSources/Actor/PawnActor/PawnActorState.h"
#include "ClientSources/Player/PlayerActor.h"

EnemyMonsterActor::EnemyMonsterActor(const EnemyMonsterActor& srcActor) : Super(srcActor)
{
	m_spEnemyRespawner = srcActor.m_spEnemyRespawner;
}

EnemyMonsterActor::~EnemyMonsterActor()
{
	
}

void EnemyMonsterActor::Startup()
{
	Super::Startup();
	AddComponent<CellActorMoveComponent>();
}

bool EnemyMonsterActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void EnemyMonsterActor::ProcessMoveDirection(const Vector2d& vMoveDir)
{
	// 이동 정보 처리
	CellActorMoveComponent* pMoveComponent = GetComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);
	if (pMoveComponent->ProcessMoveDirection(vMoveDir, true) == false)
	{
		return;
	}

	ChangeActorStateDynamicSprite<PawnActorIdleState>();
}

bool EnemyMonsterActor::CheckMovingState() const
{
	return (IsSamePawnActorState<PawnActorIdleState>());
}

void EnemyMonsterActor::DecreaseEnemyCountToEnemyRespawner() const
{
	if (m_spEnemyRespawner.expired() == false)
	{
		m_spEnemyRespawner.lock()->DecreaseEnemyCount();
	}
}