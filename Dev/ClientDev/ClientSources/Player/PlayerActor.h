// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor : public ActionableActor
{
	DECLARE_PIMPL;
	using Super = ActionableActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessMoveDirection(const Vector2d& vMoveDir) override;
	virtual void ProcessDefense() override;
	virtual void ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker) override;

	virtual void InitializeActorStateTable() override;
	virtual void RegisterStateOnBidirectional() override;

	virtual bool IsLocalPlayer() const = 0;

public:
	void LoadAndStartupPlayerSprite();
};