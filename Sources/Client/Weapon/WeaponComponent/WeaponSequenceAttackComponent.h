// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WeaponSequenceAttackComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	WeaponSequenceAttackComponent();
	virtual ~WeaponSequenceAttackComponent();

	virtual bool Update(float deltaSeconds) override;
	virtual ComponentPtr CreateClone() override;
};