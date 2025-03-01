// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(PawnActorStateIdCounter);

class PawnActor;

class PawnActorState : public ICoreLoop
{
public:
	PawnActorState(PawnActor* pOwner);
	virtual ~PawnActorState();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	PawnActor* GetOwner() const { return m_pOwner; }

private:
	PawnActor* m_pOwner = nullptr;
};

class PawnActorIdleState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~PawnActorIdleState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
};

class PawnActorWalkState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~PawnActorWalkState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
};

class PawnActorAttackState : public PawnActorState
{
	DECLARE_COMPILETIME_ID;
	using Super = PawnActorState;

public:
	using Super::Super;
	virtual ~PawnActorAttackState();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
};