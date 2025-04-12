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

	// 투사체에 이걸 도입할지부터 생각해보고 결정
	bool UpdateForLocal(float deltaSeconds);
};