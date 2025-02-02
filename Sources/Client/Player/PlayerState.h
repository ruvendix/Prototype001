// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(PlayerStateIdCount);

class PlayerActor;

class PlayerState
{
public:
	PlayerState(PlayerActor* pOwner);
	virtual ~PlayerState();

	virtual PlayerStatePtr ImmediatelyChangePlayerState() = 0;
	virtual void UpdateState(float deltaSeconds) = 0;

	PlayerActor* GetOwner() const { return m_pOwner; }

private:
	PlayerActor* m_pOwner = nullptr;
};

class PlayerIdleState : public PlayerState
{
	DECLARE_COMPILETIME_ID;
	using Super = PlayerState;

public:
	using Super::Super;

	virtual PlayerStatePtr ImmediatelyChangePlayerState() override;
	virtual void UpdateState(float deltaSeconds) override;
};

class PlayerWalkState : public PlayerState
{
	DECLARE_COMPILETIME_ID;
	using Super = PlayerState;

public:
	using Super::Super;

	virtual PlayerStatePtr ImmediatelyChangePlayerState() override;
	virtual void UpdateState(float deltaSeconds) override;
};