// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Weapon/WeaponEnums.h"

class ActionableActor : public CommunicationActor
{
	using Super = CommunicationActor;

public:
	using Super::Super;
	ActionableActor(const ActionableActor& src);
	virtual ~ActionableActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	void CreateAndPossessWeapon(uint32 weaponId, EWeaponSlotType weaponSlot);

	const WeaponActorPtr& GetWeapon(EWeaponSlotType weaponSlot) const { return m_arrWeapon[TO_NUM(weaponSlot)]; }
	const WeaponActorPtr& GetCurrentWeapon() const { return m_arrWeapon[TO_NUM(m_currentWeaponSlot)]; }

private:
	EWeaponSlotType m_currentWeaponSlot = EWeaponSlotType::Primary;
	std::array<WeaponActorPtr, TO_NUM(EWeaponSlotType::Count)> m_arrWeapon;
};