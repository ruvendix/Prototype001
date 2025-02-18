// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WeaponActor.h"

#include "Engine/Actor/PawnActor/PawnActorState.h"

WeaponActor::~WeaponActor()
{

}

void WeaponActor::Startup()
{
	Super::Startup();
}

bool WeaponActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void WeaponActor::Cleanup()
{
	Super::Cleanup();
}

void WeaponActor::ApplyDynamicSpriteToOwner()
{
	const std::shared_ptr<PawnActor>& spWeaponOwner = m_spWeaponOwner.lock();
	ASSERT_LOG_RETURN(spWeaponOwner != nullptr);

	const DynamicSpritePtr& spChangedWeaponDynamicSprite = FindActorStateLookAtDynamicSprite<PawnActorIdleState>(spWeaponOwner->GetActorLookAtDirection());
	spWeaponOwner->ChangeActorDynamicSpriteByExternal(spChangedWeaponDynamicSprite);
}