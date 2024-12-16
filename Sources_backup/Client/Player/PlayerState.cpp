// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerState.h"

#include "PlayerActor.h"
#include "PlayerImplementation.h"

PlayerActor* PlayerState::BringPlayerActor() const
{
	PlayerImplementation* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);
	return (pOwner->GetOwner());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerIdleState::ProcessPlayerInput()
{
	// Idle�̴ϱ� Walk�� ��ü�ϴ� �� ����!
	DEFAULT_TRACE_LOG("Idle �׽�Ʈ");

	PlayerActor* pPlayerActor = BringPlayerActor();
	ASSERT_LOG(pPlayerActor != nullptr);
	SceneActorMoveComponent* pMoveComponent = pPlayerActor->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoving(true);

	// � ���·� ��ü������ ���ø����� �ĳ���
	GetOwner()->ReserveChangePlayerState<PlayerWalkState>();
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
	PlayerActor* pPlayerActor = BringPlayerActor();
	ASSERT_LOG(pPlayerActor != nullptr);

	// �̵� ������Ʈ
	SceneActorMoveComponent* pMoveComponent = pPlayerActor->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->UpdateMovePosition(deltaSeconds);

	if (pMoveComponent->CheckGoalPosition() == true)
	{
		pMoveComponent->SetMoving(false);

		TransformComponent* pTransformComponent = pPlayerActor->BringTransformComponent();
		pTransformComponent->SetPosition(pMoveComponent->GetMovePosition());

		// ��������Ʈ ��ȯ
		const DynamicSpritePtr& spPlayerIdleLeftSprite = ResourceMananger::I()->FindDynamicSprite("PlayerIdleLeft");
		ASSERT_LOG_RETURN(spPlayerIdleLeftSprite != nullptr);

		DynamicSpriteComponent* pDynamicSpriteComponent = pPlayerActor->FindComponent<DynamicSpriteComponent>();
		ASSERT_LOG(pDynamicSpriteComponent != nullptr);
		pDynamicSpriteComponent->ApplyDynamicSprite(spPlayerIdleLeftSprite);

		// ���� ��ȯ
		GetOwner()->ReserveChangePlayerState<PlayerIdleState>();
	}
}