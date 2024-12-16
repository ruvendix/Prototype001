// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor;

class PlayerImplementation : public EventListener
{
	DECLARE_EVENT_HANDLER(PlayerImplementation)

public:
	PlayerImplementation(PlayerActor* pOwner);
	virtual ~PlayerImplementation();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	void ProcessPlayerInput();

	template <typename TPlayerState>
	void ReserveChangePlayerState()
	{
		PlayerStatePtr spNextPlayerState = std::make_shared<TPlayerState>(this);

		EventArgs eventArgs;
		eventArgs.push_back(spNextPlayerState);

		EventManager::I()->TriggerEvent(EEventId::ChangeState, eventArgs, this);
	}

	PlayerActor* GetOwner() const { return m_pOwner; }

private:
	void OnChangeState(const EventArgs& eventArgs);

private:
	PlayerActor* m_pOwner = nullptr;
	PlayerStatePtr m_spPlayerState = nullptr;
};