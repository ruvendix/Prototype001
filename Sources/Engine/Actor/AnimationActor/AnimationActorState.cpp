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

void AnimationActorState::Startup()
{

}

bool AnimationActorState::Update(float deltaSeconds)
{
	return true;
}

void AnimationActorState::Cleanup()
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorIdleState, AnimationActorStateIdCounter)

AnimationActorIdleState::~AnimationActorIdleState()
{

}

void AnimationActorIdleState::Startup()
{
	GetOwner()->ChangeActorStateDynamicSprite<AnimationActorIdleState>();
}

bool AnimationActorIdleState::Update(float deltaSeconds)
{
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorWalkState, AnimationActorStateIdCounter)

AnimationActorWalkState::~AnimationActorWalkState()
{

}

void AnimationActorWalkState::Startup()
{
	GetOwner()->ChangeActorStateDynamicSprite<AnimationActorWalkState>();
}

bool AnimationActorWalkState::Update(float deltaSeconds)
{
	AnimationActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// �̵� ������Ʈ
	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	if (pMoveComponent->TryCheckValidateGoalPosition(deltaSeconds, true) == true)
	{
		pMoveComponent->ResetMoveDirectionVector();

		// �̵��� �Ϸ�Ǿ����� ������ �� ��ǥ�� ���� �� ��ǥ�� ����
		pMoveComponent->ApplyDestinationDataToOwner();

		// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
		pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		// Idle ���·� ��ȯ
		pOwner->ReserveChangeNextState<AnimationActorIdleState>();
		DEFAULT_TRACE_LOG("(�ȱ� -> �⺻) ���·� ��ȯ!");
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(AnimationActorAttackState, AnimationActorStateIdCounter)

AnimationActorAttackState::~AnimationActorAttackState()
{

}

void AnimationActorAttackState::Startup()
{
	GetOwner()->ChangeActorStateDynamicSprite<AnimationActorAttackState>();
}

bool AnimationActorAttackState::Update(float deltaSeconds)
{
	AnimationActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = pOwner->FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	if (pDynamicSpriteComponent->IsDynamicSpriteEnd() == false)
	{
		return false;
	}

#pragma region ���ع��� ���Ͱ� �ִ���?
	const Position2d& forwardCellPos = pOwner->CalculateForwardCellPosition();

	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	const ActorPtr& spVictimActor = pCurrentScene->FindCellActor(EActorLayerType::Creature, forwardCellPos, nullptr);
	if (spVictimActor != nullptr)
	{
		spVictimActor->ProcessDamaged();
	}
#pragma endregion

	// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
	pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

	// Idle ���·� ��ȯ
	pOwner->ReserveChangeNextState<AnimationActorIdleState>();

	DEFAULT_TRACE_LOG("(���� -> �⺻) ���·� ��ȯ!");
	return true;
}