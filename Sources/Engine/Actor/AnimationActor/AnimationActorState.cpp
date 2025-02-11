// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "AnimationActorState.h"

#include "AnimationActor.h"

DEFINE_COMPILETIMER_COUNTER(AnimationActorStateIdCounter);

AnimationActorState::AnimationActorState(AnimationActor* pOwner)
{
	m_pOwner = pOwner;
}

AnimationActorState::~AnimationActorState()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorIdleState, AnimationActorStateIdCounter)

AnimationActorIdleState::~AnimationActorIdleState()
{

}

void AnimationActorIdleState::UpdateState(float deltaSeconds)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorWalkState, AnimationActorStateIdCounter)

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
DEFINE_COMPILETIME_ID(AnimationActorAttackState, AnimationActorStateIdCounter)

AnimationActorAttackState::~AnimationActorAttackState()
{

}

void AnimationActorAttackState::UpdateState(float deltaSeconds)
{
	AnimationActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = pOwner->FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	if (pDynamicSpriteComponent->IsAnimationEnd() == false)
	{
		return;
	}

#pragma region ���ع��� ���Ͱ� �ִ���?
	const Position2d& forwardCellPos = pOwner->CalculateForwardCellPosition();

	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);
	const ActorPtr& spVictimActor = pCurrentScene->FindAnyCellActor(EActorLayerType::Creature, forwardCellPos);
	if (spVictimActor != nullptr)
	{
		spVictimActor->ProcessDamaged();
	}
#pragma endregion

	// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
	pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

	// Idle ���·� ��ȯ
	pOwner->ReserveNextPlayerState<AnimationActorIdleState>();

	DEFAULT_TRACE_LOG("(���� -> �⺻) ���·� ��ȯ!");
}