// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WeaponActor : public CommunicationActor
{
	DECLARE_PIMPL;
	using Super = CommunicationActor;

	static const int32 HAS_NOT_PROJECTILE = -1;

public:
	using Super::Super;

	WeaponActor(const WeaponActor& srcActor);
	virtual ~WeaponActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual ActorPtr CreateClone() override;

public:
	void ApplyDynamicSpriteToOwner();
	bool TryCreateProjectile() const;

	void SetWeaponOwner(const std::shared_ptr<ActionableActor>& spWeaponOwner) { m_spWeaponOwner = spWeaponOwner; }
	std::shared_ptr<ActionableActor> GetWeaponOwner() const { return (m_spWeaponOwner.lock()); }

	void SetProjectileId(int32 projectileId) { m_projectileId = projectileId; }

private:
	int32 m_projectileId = HAS_NOT_PROJECTILE;
	std::weak_ptr<ActionableActor> m_spWeaponOwner;
};