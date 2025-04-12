// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ActionableActor.h"

ActionableActor::ActionableActor(const ActionableActor& src) : Super(src)
{

}

ActionableActor::~ActionableActor()
{

}

void ActionableActor::Startup()
{
	Super::Startup();
}

bool ActionableActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void ActionableActor::Cleanup()
{
	Super::Cleanup();
}

void ActionableActor::CreateAndPossessWeapon(uint32 weaponId, EWeaponSlotType weaponSlot)
{
	const WeaponActorPtr& spWeapon = WeaponFactory::I()->CreateWeaponActor(weaponId);
	spWeapon->SetWeaponOwner(std::dynamic_pointer_cast<ActionableActor>(shared_from_this()));

	m_arrWeapon[TO_NUM(weaponSlot)] = spWeapon;
}