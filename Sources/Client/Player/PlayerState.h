// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor;

class PlayerState
{
public:
	PlayerState(PlayerActor* pOwner);
	virtual ~PlayerState();

	virtual void ProcessPlayerInput() = 0;
	virtual void UpdateState(float deltaSeconds) = 0;

	PlayerActor* GetOwner() const { return m_pOwner; }

private:
	PlayerActor* m_pOwner = nullptr;
};

class PlayerIdleState : public PlayerState
{
	DECLARE_COMPILETIME_ID(PlayerIdleState)
	using Super = PlayerState;

public:
	using Super::Super;

	virtual void ProcessPlayerInput() override;
	virtual void UpdateState(float deltaSeconds) override;
};

class PlayerWalkState : public PlayerState
{
	DECLARE_COMPILETIME_ID(PlayerWalkState)
	using Super = PlayerState;

public:
	using Super::Super;

	virtual void ProcessPlayerInput() override;
	virtual void UpdateState(float deltaSeconds) override;
};