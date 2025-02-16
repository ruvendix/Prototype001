// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Actor/AnimationActor/AnimationActorState.h"

class AnimationActor;

class EnmeyMonsterIdleState : public AnimationActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = AnimationActorState;

public:
	using Super::Super;
	virtual ~EnmeyMonsterIdleState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

public:
	void ProcessIdleStateByRandomMove() const;
};

class EnmeyMonsterChaseState : public AnimationActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = AnimationActorState;

public:
	using Super::Super;
	virtual ~EnmeyMonsterChaseState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

public:
	void ProcessChaseStateByNavigationPath() const;
};

class EnmeyMonsterAttackState : public AnimationActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = AnimationActorState;

public:
	using Super::Super;
	virtual ~EnmeyMonsterAttackState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
};