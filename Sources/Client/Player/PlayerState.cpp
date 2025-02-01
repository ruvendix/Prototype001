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
	// Idle이니까 Walk로 교체하는 게 목적!
	DEFAULT_TRACE_LOG("걷는 상태로 전환!");
	return (std::make_shared<PlayerWalkState>(GetOwner()));
}

void PlayerIdleState::UpdateState(float deltaSeconds)
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
PlayerStatePtr PlayerWalkState::ImmediatelyChangePlayerState()
{
	DEFAULT_TRACE_LOG("걷는 상태일 때는 즉시 전환하지 않음!");
	return nullptr;
}

void PlayerWalkState::UpdateState(float deltaSeconds)
{
	PlayerActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// 이동 컴포넌트
	SceneActorMoveComponent* pMoveComponent = pOwner->FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == true)
	{
		pMoveComponent->ResetMoveDirection();

		TransformComponent* pTransformComponent = pOwner->BringTransformComponent();
		pTransformComponent->SetPosition(pMoveComponent->GetDestinationWorldPosition());

		// 스프라이트 전환
		const std::string& strIdleSpriteName = pOwner->FindPlayerIdleSpriteString(pMoveComponent->GetLookAtType());
		pOwner->ChangePlayerSprite(strIdleSpriteName);

		// Idle 상태로 전환
		pOwner->ReserveNextPlayerState<PlayerIdleState>();
	}
}