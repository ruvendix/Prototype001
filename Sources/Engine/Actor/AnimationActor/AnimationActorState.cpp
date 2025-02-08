// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "AnimationActorState.h"

#include "AnimationActor.h"

DEFINE_COMPILETIMER_COUNTER(AnimationActorStateIdCount);

AnimationActorState::AnimationActorState(AnimationActor* pOwner)
{
	m_pOwner = pOwner;
}

AnimationActorState::~AnimationActorState()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorIdleState, AnimationActorStateIdCount)

AnimationActorIdleState::~AnimationActorIdleState()
{

}

void AnimationActorIdleState::UpdateState(float deltaSeconds)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorWalkState, AnimationActorStateIdCount)

AnimationActorWalkState::~AnimationActorWalkState()
{

}

void AnimationActorWalkState::UpdateState(float deltaSeconds)
{
	AnimationActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// 이동 컴포넌트
	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == true)
	{
		pMoveComponent->ResetMoveDirection();

		// 이동이 완료되었으니 목적지 셀 좌표를 현재 셀 좌표로 적용
		pMoveComponent->ApplyDestinationDataToOwner();

		// 스프라이트는 현재 프레임에서 바로 전환
		pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		// Idle 상태로 전환
		pOwner->ReserveNextPlayerState<AnimationActorIdleState>();
		DEFAULT_TRACE_LOG("(걷기 -> 기본) 상태로 전환!");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorAttackState, AnimationActorStateIdCount)

AnimationActorAttackState::~AnimationActorAttackState()
{

}

void AnimationActorAttackState::UpdateState(float deltaSeconds)
{
	AnimationActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = pOwner->FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	if (pDynamicSpriteComponent->IsAnimationEnd())
	{
		// 스프라이트는 현재 프레임에서 바로 전환
		pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		// Idle 상태로 전환
		pOwner->ReserveNextPlayerState<AnimationActorIdleState>();
		DEFAULT_TRACE_LOG("(공격 -> 기본) 상태로 전환!");
	}
}