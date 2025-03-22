// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class SnakeActor : public EnemyMonsterActor
{
	DECLARE_PIMPL;
	DECLARE_COMPILETIME_ID;

	using Super = EnemyMonsterActor;

public:
	using Super::Super;
	virtual ~SnakeActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual ActorPtr CreateClone() override;
	virtual void RegisterStateOnBidirectional() override;

	virtual void ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker) override;
};