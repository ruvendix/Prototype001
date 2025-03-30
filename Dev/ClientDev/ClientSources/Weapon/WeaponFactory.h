// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WeaponFactory : public ICoreLoop
{
	DECLARE_SINGLETON(WeaponFactory)
	DECLARE_PIMPL;

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	std::shared_ptr<WeaponActor> CreateWeaponActor(int32 weaponId) const;
	const WeaponActor* FindWeaponActorRawPointer(int32 weaponId) const;

	// 일단은 무기 공장에 넣어봄
	std::shared_ptr<ProjectileActor> CreateProjectileActor(int32 projectileId) const;
	const ProjectileActor* FindProjectileActorRawPointer(int32 projectileId) const;
	const std::shared_ptr<ProjectileActor>& FindProjectileActor(int32 projectileId) const;

public:
	void ReserveCreateProjectileActor(const ProjectileSpawnInfo& projectileSpawnInfo);

private:
	void OnCreateProjectileActor(const ProjectileSpawnInfo& projectileSpawnInfo);

private:
	Event<const ProjectileSpawnInfo&> m_createProjectileActorEvent;

	std::unordered_map<int32, std::shared_ptr<WeaponActor>> m_mapPrototypeWeapon;
	std::unordered_map<int32, std::shared_ptr<ProjectileActor>> m_mapPrototypeProjectile;
};