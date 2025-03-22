// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterState.h"

#include "ClientSources/Player/PlayerActor.h"
#include "ClientSources/EnemyMonster/EnemyMonsterComponent/EnemyMonsterAttackComponent.h"

DEFINE_COMPILETIME_ID(EnmeyMonsterIdleState, PawnActorStateIdCounter)
EnmeyMonsterIdleState::~EnmeyMonsterIdleState()
{

}

void EnmeyMonsterIdleState::Startup()
{

}

bool EnmeyMonsterIdleState::Update(float deltaSeconds)
{
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterChaseState, PawnActorStateIdCounter)
EnmeyMonsterChaseState::~EnmeyMonsterChaseState()
{

}

void EnmeyMonsterChaseState::Startup()
{

}

bool EnmeyMonsterChaseState::Update(float deltaSeconds)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterAttackState, PawnActorStateIdCounter)
EnmeyMonsterAttackState::~EnmeyMonsterAttackState()
{

}

void EnmeyMonsterAttackState::Startup()
{

}

bool EnmeyMonsterAttackState::Update(float deltaSeconds)
{
//	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
//	if (spEnemyMonsterOwner == nullptr)
//	{
//		return false;
//	}
//
//	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
//	ASSERT_LOG(pCurrentScene != nullptr);
//
//	EnemyMonsterAttackComponent* pEnemyMonsterAttackComponent = spEnemyMonsterOwner->FindComponent<EnemyMonsterAttackComponent>();
//	ASSERT_LOG_RETURN_VALUE(pEnemyMonsterAttackComponent != nullptr, false);
//
//	// 바로 앞에 플레이어가 없다면 추적 상태나 Idle로 변경
//	const Position2d& forwardCellPos = spEnemyMonsterOwner->CalculateForwardCellPosition();
//	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = pCurrentScene->FindExactTypeBaseOfCellActor<PlayerActor>(EActorLayerType::Creature, forwardCellPos);
//	if (spFoundAttackablePlayerActor != nullptr)
//	{
//		pEnemyMonsterAttackComponent->ActivateAttackIfAttackable(spFoundAttackablePlayerActor);
//		pEnemyMonsterAttackComponent->SetAttackDelayTime(1.0f);
//		return false;
//	}
//	
//	pEnemyMonsterAttackComponent->OffAttackTimer();
//
//#pragma region 여전히 플레이어를 추적할 수 있는지 확인
//	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
//	if (spFoundChaseablePlayerActor == nullptr)
//	{
//		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
//		DEFAULT_TRACE_LOG("몬스터 (공격 -> Idle) 상태로 전환!");
//
//		return false;
//	}
//#pragma endregion
//
//	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
//	DEFAULT_TRACE_LOG("몬스터 (공격 -> 추적) 상태로 전환!");

	return true;
}