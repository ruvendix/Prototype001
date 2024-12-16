// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor : public SceneActor
{
	DECLARE_PIMPL;
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
	void ReserveChangePlayerState(EventArgs& outEventArgs)
	{
		PlayerStatePtr spNextPlayerState = std::make_shared<TPlayerState>(this);

		EventArgs eventArgs;
		eventArgs.push_back(spNextPlayerState);

		outEventArgs = eventArgs;
	}

private:
	void OnKeyboardDown_Left();
	void OnKeyboardDown_Right();
	void OnKeyboardDown_Down();
	void OnKeyboardDown_Up();

private:
	PlayerStatePtr m_spPlayerState = nullptr;
};