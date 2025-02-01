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
PlayerStatePtr PlayerIdleState::ImmediatelyChangePlayerState()
{
	// Idle�̴ϱ� Walk�� ��ü�ϴ� �� ����!
	DEFAULT_TRACE_LOG("�ȴ� ���·� ��ȯ!");
	return (std::make_shared<PlayerWalkState>(GetOwner()));
}

void PlayerIdleState::UpdateState(float deltaSeconds)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
PlayerStatePtr PlayerWalkState::ImmediatelyChangePlayerState()
{
	DEFAULT_TRACE_LOG("�ȴ� ������ ���� ��� ��ȯ���� ����!");
	return nullptr;
}

void PlayerWalkState::UpdateState(float deltaSeconds)
{
	PlayerActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// �̵� ������Ʈ
	SceneActorMoveComponent* pMoveComponent = pOwner->FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == true)
	{
		TransformComponent* pTransformComponent = pOwner->BringTransformComponent();
		pTransformComponent->SetPosition(pMoveComponent->GetDestinationWorldPosition());

		// ��������Ʈ ��ȯ
		const std::string& strIdleSpriteName = pOwner->FindPlayerIdleSpriteString(pMoveComponent->GetMoveDirection());
		pOwner->ChangePlayerSprite(strIdleSpriteName);

		// Idle ���·� ��ȯ
		pOwner->ReserveNextPlayerState<PlayerIdleState>();
	}
}