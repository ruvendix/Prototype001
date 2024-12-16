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

	// � ���·� ��ü������ ���ø����� �ĳ���
	EventArgs eventArgs;
	pOwner->ReserveChangePlayerState<PlayerWalkState>(eventArgs);
	pOwner->TriggerChangePlayerState(eventArgs);
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

	if (pMoveComponent->CheckGoalPosition() == true)
	{
		pMoveComponent->SetMoving(false);

		TransformComponent* pTransformComponent = pOwner->BringTransformComponent();
		pTransformComponent->SetPosition(pMoveComponent->GetMovePosition());

		// ��������Ʈ ��ȯ
		const std::string& strIdleSpriteName = pOwner->FindPlayerIdleSpriteString(pMoveComponent->GetMoveDirection());
		pOwner->ChangePlayerSprite(strIdleSpriteName);

		// ���� ��ȯ
		EventArgs eventArgs;
		pOwner->ReserveChangePlayerState<PlayerIdleState>(eventArgs);
		pOwner->TriggerChangePlayerState(eventArgs);
	}
}