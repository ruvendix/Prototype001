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
	// Idle이니까 Walk로 교체하는 게 목적!
	DEFAULT_TRACE_LOG("Idle 테스트");

	PlayerActor* pPlayerActor = BringPlayerActor();
	ASSERT_LOG(pPlayerActor != nullptr);
	SceneActorMoveComponent* pMoveComponent = pPlayerActor->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoving(true);

	// 어떤 상태로 교체할지는 템플릿으로 쳐넣음
	GetOwner()->ReserveChangePlayerState<PlayerWalkState>();
}

void PlayerIdleState::UpdateState(float deltaSeconds)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerWalkState::ProcessPlayerInput()
{
	DEFAULT_TRACE_LOG("워크로 전환!");
}

void PlayerWalkState::UpdateState(float deltaSeconds)
{
	PlayerActor* pPlayerActor = BringPlayerActor();
	ASSERT_LOG(pPlayerActor != nullptr);

	// 이동 컴포넌트
	SceneActorMoveComponent* pMoveComponent = pPlayerActor->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->UpdateMovePosition(deltaSeconds);

	if (pMoveComponent->CheckGoalPosition() == true)
	{
		pMoveComponent->SetMoving(false);

		TransformComponent* pTransformComponent = pPlayerActor->BringTransformComponent();
		pTransformComponent->SetPosition(pMoveComponent->GetMovePosition());

		// 스프라이트 전환
		const DynamicSpritePtr& spPlayerIdleLeftSprite = ResourceMananger::I()->FindDynamicSprite("PlayerIdleLeft");
		ASSERT_LOG_RETURN(spPlayerIdleLeftSprite != nullptr);

		DynamicSpriteComponent* pDynamicSpriteComponent = pPlayerActor->FindComponent<DynamicSpriteComponent>();
		ASSERT_LOG(pDynamicSpriteComponent != nullptr);
		pDynamicSpriteComponent->ApplyDynamicSprite(spPlayerIdleLeftSprite);

		// 상태 전환
		GetOwner()->ReserveChangePlayerState<PlayerIdleState>();
	}
}