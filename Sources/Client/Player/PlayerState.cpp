// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerState.h"

#include "PlayerActor.h"

PlayerState::PlayerState(PlayerActor* pOwner)
{
	m_pOwner = pOwner;
}

PlayerState::~PlayerState()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerIdleState::ProcessPlayerInput()
{
	// Idle�̴ϱ� Walk�� ��ü�ϴ� �� ����!
	DEFAULT_TRACE_LOG("Idle �׽�Ʈ");

	PlayerActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	SceneActorMoveComponent* pMoveComponent = pOwner->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoving(true);

	// Walk ���·� ��ȯ
	pOwner->ReserveChangePlayerState<PlayerWalkState>();
}

void PlayerIdleState::UpdateState(float deltaSeconds)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerWalkState::ProcessPlayerInput()
{
	DEFAULT_TRACE_LOG("��ũ�� ��ȯ!");
}

void PlayerWalkState::UpdateState(float deltaSeconds)
{
	PlayerActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// �̵� ������Ʈ
	SceneActorMoveComponent* pMoveComponent = pOwner->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->UpdateMovePosition(deltaSeconds);

	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == true)
	{
		pMoveComponent->SetMoving(false);

		TransformComponent* pTransformComponent = pOwner->BringTransformComponent();
		pTransformComponent->SetPosition(pMoveComponent->GetDestinationWorldPosition());

		// ��������Ʈ ��ȯ
		const std::string& strIdleSpriteName = pOwner->FindPlayerIdleSpriteString(pMoveComponent->GetMoveDirection());
		pOwner->ChangePlayerSprite(strIdleSpriteName);

		// Idle ���·� ��ȯ
		pOwner->ReserveChangePlayerState<PlayerIdleState>();
	}
}