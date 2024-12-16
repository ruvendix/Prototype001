// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "PlayerEvent.h"

class PlayerActor : public SceneActor
{
	DECLARE_PIMPL;
	DEFINE_EVENT_HANDLER;

	using Super = SceneActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	void ProcessPlayerInput();
	void ChangePlayerSprite(const std::string& strNextPlayerSprite);

	void TriggerChangePlayerState(const EventArgs& eventArgs);

	const std::string& FindPlayerIdleSpriteString(ESceneActorMoveDirection moveDir) const;
	const std::string& FindPlayerWalkSpriteString(ESceneActorMoveDirection moveDir) const;

	template <typename TPlayerState>
	void ReserveChangePlayerState()
	{
		PlayerStatePtr spNextPlayerState = std::make_shared<TPlayerState>(this);
		ReserveEvent<PlayerStateChangeEvent>(spNextPlayerState);
	}

private:
	void OnKeyboardDown_Left();
	void OnKeyboardDown_Right();
	void OnKeyboardDown_Down();
	void OnKeyboardDown_Up();

	void OnChangeState(const EventArgs& eventArgs);

private:
	PlayerStatePtr m_spPlayerState = nullptr;
};