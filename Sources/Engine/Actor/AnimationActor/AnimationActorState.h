// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(AnimationActorStateIdCount);

class AnimationActor;

class AnimationActorState
{
public:
	AnimationActorState(AnimationActor* pOwner);
	virtual ~AnimationActorState();

	virtual void UpdateState(float deltaSeconds) = 0;

public:
	AnimationActor* GetOwner() const { return m_pOwner; }

private:
	AnimationActor* m_pOwner = nullptr;
};

class AnimationActorIdleState : public AnimationActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = AnimationActorState;

public:
	using Super::Super;
	virtual ~AnimationActorIdleState();

public:
	virtual void UpdateState(float deltaSeconds) override;
};

class AnimationActorWalkState : public AnimationActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = AnimationActorState;

public:
	using Super::Super;
	virtual ~AnimationActorWalkState();

public:
	virtual void UpdateState(float deltaSeconds) override;
};

class AnimationActorAttackState : public AnimationActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = AnimationActorState;

public:
	using Super::Super;
	virtual ~AnimationActorAttackState();

	virtual void UpdateState(float deltaSeconds) override;
};