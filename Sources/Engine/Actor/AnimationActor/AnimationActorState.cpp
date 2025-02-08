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

	// �̵� ������Ʈ
	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == true)
	{
		pMoveComponent->ResetMoveDirection();

		// �̵��� �Ϸ�Ǿ����� ������ �� ��ǥ�� ���� �� ��ǥ�� ����
		pMoveComponent->ApplyDestinationDataToOwner();

		// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
		pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		// Idle ���·� ��ȯ
		pOwner->ReserveNextPlayerState<AnimationActorIdleState>();
		DEFAULT_TRACE_LOG("(�ȱ� -> �⺻) ���·� ��ȯ!");
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
		// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
		pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		// Idle ���·� ��ȯ
		pOwner->ReserveNextPlayerState<AnimationActorIdleState>();
		DEFAULT_TRACE_LOG("(���� -> �⺻) ���·� ��ȯ!");
	}
}