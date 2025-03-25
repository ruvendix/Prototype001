// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PawnActorState.h"

#include "PawnActor.h"

DEFINE_COMPILETIMER_COUNTER(PawnActorStateIdCounter);

PawnActorState::PawnActorState(PawnActor* pOwner)
{
	m_pOwner = pOwner;
}

PawnActorState::~PawnActorState()
{

}

void PawnActorState::Startup()
{

}

bool PawnActorState::Update(float deltaSeconds)
{
	return true;
}

void PawnActorState::Cleanup()
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(PawnActorIdleState, PawnActorStateIdCounter)

PawnActorIdleState::~PawnActorIdleState()
{

}

void PawnActorIdleState::Startup()
{
	GetOwner()->ChangeActorStateDynamicSprite<PawnActorIdleState>();
}

bool PawnActorIdleState::Update(float deltaSeconds)
{
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(PawnActorWalkState, PawnActorStateIdCounter)

PawnActorWalkState::~PawnActorWalkState()
{

}

void PawnActorWalkState::Startup()
{
	GetOwner()->ChangeActorStateDynamicSprite<PawnActorWalkState>();
}

bool PawnActorWalkState::Update(float deltaSeconds)
{
	PawnActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// �̵� ������Ʈ
	CellActorMoveComponent* pMoveComponent = pOwner->GetComponent<CellActorMoveComponent>();
	if (pMoveComponent->IsZeroMoveDirectionVector())
	{
		// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
		pOwner->ChangeActorStateDynamicSprite<PawnActorIdleState>();

		// Idle ���·� ��ȯ
		pOwner->ReserveChangeNextState<PawnActorIdleState>();
		DEFAULT_TRACE_LOG("(�ȱ� -> �⺻) ���·� ��ȯ!");
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(PawnActorAttackState, PawnActorStateIdCounter)

PawnActorAttackState::~PawnActorAttackState()
{

}

void PawnActorAttackState::Startup()
{
	GetOwner()->ChangeActorStateDynamicSprite<PawnActorAttackState>();
}

bool PawnActorAttackState::Update(float deltaSeconds)
{
	PawnActor* pAttacker = GetOwner();
	ASSERT_LOG(pAttacker != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = pAttacker->GetComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	if (pDynamicSpriteComponent->CheckDynamicSpriteEnd() == false)
	{
		return false;
	}

#pragma region ���ع��� ���Ͱ� �ִ���?
	const Position2d& forwardCellPos = pAttacker->CalculateForwardCellPosition();

	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	const PawnActorPtr& spVictim = std::dynamic_pointer_cast<PawnActor>(pCurrentScene->FindCellActor(EActorLayerType::Creature, forwardCellPos, nullptr));
	if (spVictim != nullptr)
	{
		spVictim->ProcessDamaged(pAttacker->SharedFromThisExactType<PawnActor>());
	}
#pragma endregion

	// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
	pAttacker->ChangeActorStateDynamicSprite<PawnActorIdleState>();

	// Idle ���·� ��ȯ
	pAttacker->ReserveChangeNextState<PawnActorIdleState>();

	DEFAULT_TRACE_LOG("(���� -> �⺻) ���·� ��ȯ!");
	return true;
}