// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/Actor/PawnActor/PawnActorState.h"

class PawnActor;

class EnmeyMonsterIdleState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~EnmeyMonsterIdleState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

public:
	void ProcessIdleStateByRandomMove() const;
};

class EnmeyMonsterChaseState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~EnmeyMonsterChaseState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

public:
	void ProcessChaseStateByNavigationPath() const;
};

class EnmeyMonsterAttackState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~EnmeyMonsterAttackState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
};