// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WeaponFactory : public ICoreLoop
{
	DECLARE_SINGLETON(WeaponFactory)
	DECLARE_PIMPL;

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	WeaponActorPtr CreateWeaponActor(uint32 weaponId) const;
	const WeaponActor* FindWeaponActorRawPointer(uint32 weaponId) const;

	// 일단은 무기 공장에 넣어봄
	std::shared_ptr<ProjectileActor> CreateProjectileActor(uint32 projectileId) const;
	const ProjectileActor* FindProjectileActorRawPointer(uint32 projectileId) const;
	const std::shared_ptr<ProjectileActor>& FindProjectileActor(uint32 projectileId) const;

private:
	Event<const ProjectileSpawnInfo&> m_createProjectileActorEvent;

	std::unordered_map<int32, WeaponActorPtr> m_mapPrototypeWeapon;
	std::unordered_map<int32, std::shared_ptr<ProjectileActor>> m_mapPrototypeProjectile;
};