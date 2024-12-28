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
	// Idle이니까 Walk로 교체하는 게 목적!
	DEFAULT_TRACE_LOG("Idle 테스트");

	PlayerActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	SceneActorMoveComponent* pMoveComponent = pOwner->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoving(true);

	// Walk 상태로 전환
	pOwner->ReserveChangePlayerState<PlayerWalkState>();
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
	PlayerActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// 이동 컴포넌트
	SceneActorMoveComponent* pMoveComponent = pOwner->FindComponent<SceneActorMoveComponent>();
	pMoveComponent->UpdateMovePosition(deltaSeconds);

	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == true)
	{
		pMoveComponent->SetMoving(false);

		TransformComponent* pTransformComponent = pOwner->BringTransformComponent();
		pTransformComponent->SetPosition(pMoveComponent->GetDestinationWorldPosition());

		// 스프라이트 전환
		const std::string& strIdleSpriteName = pOwner->FindPlayerIdleSpriteString(pMoveComponent->GetMoveDirection());
		pOwner->ChangePlayerSprite(strIdleSpriteName);

		// Idle 상태로 전환
		pOwner->ReserveChangePlayerState<PlayerIdleState>();
	}
}