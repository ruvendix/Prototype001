// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor : public GameEntityActor
{
	DECLARE_PIMPL;
	using Super = GameEntityActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessMoveDirection(const Vector2d& vMoveDir) override;
	virtual void ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker) override;
	
public:
	void LoadAndStartupPlayerSprite();
	const std::shared_ptr<WeaponActor>& GetWeaponActor(EWeaponSlotType weaponSlot) const { return m_arrWeaponActor[TO_NUM(weaponSlot)]; }

private:
	std::array<std::shared_ptr<WeaponActor>, TO_NUM(EWeaponSlotType::Count)> m_arrWeaponActor;
};