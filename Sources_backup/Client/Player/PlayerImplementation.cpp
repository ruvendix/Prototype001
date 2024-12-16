// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerImplementation.h"

#include "PlayerState.h"

DEFINE_EVENT_HANDLER(PlayerImplementation);

PlayerImplementation::PlayerImplementation(PlayerActor* pOwner)
{
	m_pOwner = pOwner;
}

PlayerImplementation::~PlayerImplementation()
{

}

void PlayerImplementation::Startup()
{
	m_spPlayerState = std::make_shared<PlayerIdleState>(this);
	RegisterEventHandler(EEventId::ChangeState, &PlayerImplementation::OnChangeState);
}

bool PlayerImplementation::Update(float deltaSeconds)
{
	m_spPlayerState->UpdateState(deltaSeconds);
	return true;
}

void PlayerImplementation::ProcessPlayerInput()
{
	m_spPlayerState->ProcessPlayerInput();
}

void PlayerImplementation::OnChangeState(const EventArgs& eventArgs)
{
	DEFAULT_TRACE_LOG("체인지 스테이트!");
	m_spPlayerState = std::any_cast<PlayerStatePtr>(eventArgs[0]);
}