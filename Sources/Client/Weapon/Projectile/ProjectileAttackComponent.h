// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class ProjectileAttackComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	ProjectileAttackComponent();
	virtual ~ProjectileAttackComponent();

	virtual bool Update(float deltaSeconds) override;
	virtual ComponentPtr CreateClone() override;
};