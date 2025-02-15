// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterState.h"

#include "Client/Player/PlayerActor.h"
#include "Client/EnemyMonster/EnemyMonsterComponent/EnemyMonsterAttackComponent.h"

DEFINE_COMPILETIME_ID(EnmeyMonsterIdleState, AnimationActorStateIdCounter)
EnmeyMonsterIdleState::~EnmeyMonsterIdleState()
{
}

void EnmeyMonsterIdleState::Startup()
{

}

bool EnmeyMonsterIdleState::Update(float deltaSeconds)
{
	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
	if (spEnemyMonsterOwner == nullptr)
	{
		return false;
	}

	// 추적 가능한 플레이어를 찾을 때만 추적 상태로 변경
	const std::shared_ptr<PlayerActor>& spFoundPlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundPlayerActor == nullptr)
	{
		return false;
	}

	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
	DEFAULT_TRACE_LOG("몬스터 (Idle -> 추적) 상태로 전환!");

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterChaseState, AnimationActorStateIdCounter)
EnmeyMonsterChaseState::~EnmeyMonsterChaseState()
{

}

void EnmeyMonsterChaseState::Startup()
{

}

bool EnmeyMonsterChaseState::Update(float deltaSeconds)
{
	AnimationActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// 이동 컴포넌트
	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pMoveComponent != nullptr, false);
	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == false)
	{
		return false;
	}

	pMoveComponent->ResetMoveDirection();

	// 이동이 완료되었으니 목적지 셀 좌표를 현재 셀 좌표로 적용
	pMoveComponent->ApplyDestinationDataToOwner();

	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(pOwner);
	if (spEnemyMonsterOwner == nullptr)
	{
		return false;
	}

#pragma region 한칸 이동하면 플레이어와 한칸 차이인지 확인
	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = spEnemyMonsterOwner->FindNearbyPlayerActor();
	if (spFoundAttackablePlayerActor != nullptr)
	{
		// 바라보는 방향 조정
		const Vec2d& vMoveDir = spEnemyMonsterOwner->CalculateMoveDirectionByCellPosition(spFoundAttackablePlayerActor->GetCellPosition());
		spEnemyMonsterOwner->ApplyMoveDirectionToLookAtDirection(vMoveDir);
		spEnemyMonsterOwner->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();

		// Attack 상태로 전환
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterAttackState>();
		DEFAULT_TRACE_LOG("몬스터 (추적 -> 공격) 상태로 전환!");
		return true;
	}
#pragma endregion

#pragma region 한칸 이동하면 여전히 플레이어를 추적할 수 있는지 확인
	// 추적 가능한 플레이어를 못 찾았으면 Idle로 변경
	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundChaseablePlayerActor == nullptr)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return false;
	}
#pragma endregion

	std::vector<Position2d> vecNavigationPos;
	EnemyMonsterActor::CalculateNavigationPath(spEnemyMonsterOwner->GetCellPosition(),
		spFoundChaseablePlayerActor->GetCellPosition(), spEnemyMonsterOwner->GetChaseRange(), vecNavigationPos);

	if (vecNavigationPos.empty() == true)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return false;
	}

	// 이동할 위치에서 현재 위치와의 차이로 방향 벡터 판단
	const Vec2d& vMoveDir = pOwner->CalculateMoveDirectionByCellPosition(vecNavigationPos[0]);

	// 이동 처리
	CellActorMoveComponent* pCellActorMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pCellActorMoveComponent != nullptr, false);
	if (pCellActorMoveComponent->ProcessMove(vMoveDir) == false)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return false;
	}

	GetOwner()->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterAttackState, AnimationActorStateIdCounter)
EnmeyMonsterAttackState::~EnmeyMonsterAttackState()
{

}

void EnmeyMonsterAttackState::Startup()
{

}

bool EnmeyMonsterAttackState::Update(float deltaSeconds)
{
	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
	if (spEnemyMonsterOwner == nullptr)
	{
		return false;
	}

	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG(pCurrentScene != nullptr);

	EnemyMonsterAttackComponent* pEnemyMonsterAttackComponent = spEnemyMonsterOwner->FindComponent<EnemyMonsterAttackComponent>();
	ASSERT_LOG_RETURN_VALUE(pEnemyMonsterAttackComponent != nullptr, false);

	// 바로 앞에 플레이어가 없다면 추적 상태나 Idle로 변경
	const Position2d& forwardCellPos = spEnemyMonsterOwner->CalculateForwardCellPosition();
	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = pCurrentScene->FindExactTypeBaseOfCellActor<PlayerActor>(EActorLayerType::Creature, forwardCellPos);
	if (spFoundAttackablePlayerActor != nullptr)
	{
		pEnemyMonsterAttackComponent->ActivateAttackIfAttackable(spFoundAttackablePlayerActor);
		pEnemyMonsterAttackComponent->SetAttackDelayTime(1.0f);
		return false;
	}
	
	pEnemyMonsterAttackComponent->OffAttackTimer();

#pragma region 여전히 플레이어를 추적할 수 있는지 확인
	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundChaseablePlayerActor == nullptr)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		DEFAULT_TRACE_LOG("몬스터 (공격 -> Idle) 상태로 전환!");

		return false;
	}
#pragma endregion

	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
	DEFAULT_TRACE_LOG("몬스터 (공격 -> 추적) 상태로 전환!");

	return true;
}