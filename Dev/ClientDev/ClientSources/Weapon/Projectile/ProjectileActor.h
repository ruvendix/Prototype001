// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class ProjectileActor : public CommunicationActor
{
	DECLARE_PIMPL;
	using Super = CommunicationActor;

public:
	using Super::Super;

	ProjectileActor(const ProjectileActor& srcActor);
	virtual ~ProjectileActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual ActorPtr CreateClone() override;
	virtual void ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker) override;

public:
	void SpawnProjectile(const ProjectileSpawnInfo& projectileSpawnInfo);

	void SetLinkedWeaponActor(const WeaponActorPtr& spLinkedWeaponActor) { m_spLinkedWeaponActor = spLinkedWeaponActor; }
	std::shared_ptr<PawnActor> GetLinkedWeaponActor() const { return m_spLinkedWeaponActor; }

private:
	// ����ü�� ���� �Ҽ��̹Ƿ� �����ϰ� ���� ����ʹ� ���Ḹ �Ǿ��ٴ� ����
	WeaponActorPtr m_spLinkedWeaponActor = nullptr;
};