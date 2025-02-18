// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WeaponFactory : public ICoreLoop
{
	DECLARE_SINGLETON(WeaponFactory)

public:
	virtual void Startup() override;

	std::shared_ptr<WeaponActor> CreateWeaponActor(int32 weaponId);
	std::shared_ptr<WeaponActor> FindWeaponActor(int32 weaponId) const;

private:
	std::unordered_map<int32, std::shared_ptr<WeaponActor>> m_mapWeaponPrototype;
};