// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor : public NetworkEntityActor
{
	DECLARE_PIMPL;
	using Super = NetworkEntityActor;

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

	virtual void ProcessWeaponAttack() = 0;

public:
	void LoadAndStartupPlayerSprite();

	const std::shared_ptr<WeaponActor>& GetWeapon(EWeaponSlotType weaponSlot) const { return m_arrWeaponActor[TO_NUM(weaponSlot)]; }
	const std::shared_ptr<WeaponActor>& GetCurrentWeapon() const { return m_arrWeaponActor[TO_NUM(m_currentWeaponSlot)]; }

private:
	EWeaponSlotType m_currentWeaponSlot = EWeaponSlotType::Primary;
	std::array<std::shared_ptr<WeaponActor>, TO_NUM(EWeaponSlotType::Count)> m_arrWeaponActor;
};