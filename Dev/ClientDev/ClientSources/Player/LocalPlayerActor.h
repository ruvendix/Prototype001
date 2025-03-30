// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "PlayerActor.h"

class InputActionValue;

class LocalPlayerActor : public PlayerActor
{
	DECLARE_PIMPL;
	using Super = PlayerActor;

public:
	using Super::Super;
	virtual ~LocalPlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessAttackAction() override;
	virtual void ProcessWeaponAttack() override;

private:
	void OnDirectionKeyHandler(const InputActionValue* pInputAction);
	void OnSpaceBarKeyHandler(const InputActionValue* pInputAction);
	void OnAKeyHandler(const InputActionValue* pInputAction);
};