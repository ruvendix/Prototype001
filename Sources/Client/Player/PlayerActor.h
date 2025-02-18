// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor : public PawnActor
{
	DECLARE_PIMPL;
	using Super = PawnActor;

public:
	using Super::Super;
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker) override;

public:
	const std::shared_ptr<WeaponActor>& GetWeaponActor(EWeaponSlotType weaponSlot) const { return m_arrWeaponActor[TO_NUM(weaponSlot)]; }

private:
	void OnDirectionKeyHandler(const InputActionValue* pInputAction);
	void OnSpaceBarKeyHandler(const InputActionValue* pInputAction);
	void OnAKeyHandler(const InputActionValue* pInputAction);

private:
	std::array<std::shared_ptr<WeaponActor>, TO_NUM(EWeaponSlotType::Count)> m_arrWeaponActor;
};