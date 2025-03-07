// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/Actor/PawnActor/PawnActorState.h"

class PawnActor;

class PlayerAttackState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~PlayerAttackState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
};

class PlayerDefenceState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~PlayerDefenceState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

public:
	bool CheckKnockbackExisted() const;
};