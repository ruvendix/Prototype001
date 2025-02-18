// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WeaponActor : public PawnActor
{
	DECLARE_PIMPL;
	using Super = PawnActor;

public:
	using Super::Super;
	virtual ~WeaponActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	void ApplyDynamicSpriteToOwner();

	void SetWeaponOwner(const std::shared_ptr<PawnActor>& spWeaponOwner) { m_spWeaponOwner = spWeaponOwner; }
	std::shared_ptr<PawnActor> GetWeaponOwner() const { return (m_spWeaponOwner.lock()); }

private:
	// 컴포넌트처럼 투사체 생성기 넣으면 되긴함
	std::weak_ptr<PawnActor> m_spWeaponOwner;
};