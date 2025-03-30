// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WeaponActor.h"

#include "EngineSources/Actor/PawnActor/PawnActorState.h"

WeaponActor::WeaponActor(const WeaponActor& srcActor) :
	Super(srcActor)
{
	m_projectileId = srcActor.m_projectileId;
}

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

ActorPtr WeaponActor::CreateClone()
{
	return (std::make_shared<WeaponActor>(*this));
}

void WeaponActor::ApplyDynamicSpriteToOwner()
{
	const std::shared_ptr<PawnActor>& spWeaponOwner = m_spWeaponOwner.lock();
	ASSERT_LOG_RETURN(spWeaponOwner != nullptr);

	const DynamicSpritePtr& spChangedWeaponDynamicSprite = FindActorStateLookAtDynamicSprite<PawnActorIdleState>(spWeaponOwner->GetActorLookAtDirection());
	spWeaponOwner->ChangeActorDynamicSpriteByExternal(spChangedWeaponDynamicSprite);
}

bool WeaponActor::TryCreateProjectile() const
{
	if (m_projectileId == HAS_NOT_PROJECTILE)
	{
		return false;
	}

	ProjectileSpawnInfo projectileSpawnInfo;
	projectileSpawnInfo.projectileId = m_projectileId;
	projectileSpawnInfo.projectileSize = Size{ 100, 100 };
	projectileSpawnInfo.flyingSpeed = 1000.0f;

	const std::shared_ptr<PawnActor>& spWeaponOwner = GetWeaponOwner();
	projectileSpawnInfo.spawnCellPos = spWeaponOwner->CalculateForwardCellPosition();
	projectileSpawnInfo.vMoveDir = spWeaponOwner->CalculateMoveDirectionByCellPosition(projectileSpawnInfo.spawnCellPos);

	WeaponFactory::I()->ReserveCreateProjectileActor(projectileSpawnInfo);
	return true;
}