// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerState.h"

#include "ClientSources/Player/PlayerActor.h"

namespace
{
	bool UpdateWeaponActor(float deltaSeconds, PawnActor* pOwner, EWeaponSlotType weaponSlot)
	{
		PlayerActor* pPlayer = dynamic_cast<PlayerActor*>(pOwner);
		ASSERT_LOG_RETURN_VALUE(pPlayer != nullptr, false);

		// 무기에서 전부 처리
		const std::shared_ptr<WeaponActor>& spWeaponActor = pPlayer->GetWeapon(weaponSlot);
		return spWeaponActor->Update(deltaSeconds);
	}
}

DEFINE_COMPILETIME_ID(PlayerAttackState, PawnActorStateIdCounter)
PlayerAttackState::~PlayerAttackState()
{

}

void PlayerAttackState::Startup()
{
	PlayerActor* pPlayer = dynamic_cast<PlayerActor*>(GetOwner());
	ASSERT_LOG(pPlayer != nullptr);

	// 현재 무기에 따라 작동
	const std::shared_ptr<WeaponActor>& spPrimaryWeapon = pPlayer->GetWeapon(EWeaponSlotType::Primary);
	if (spPrimaryWeapon == nullptr)
	{
		return;
	}

	spPrimaryWeapon->ApplyDynamicSpriteToOwner();
}

bool PlayerAttackState::Update(float deltaSeconds)
{
	return (UpdateWeaponActor(deltaSeconds, GetOwner(), EWeaponSlotType::Primary));
}
///////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(PlayerDefenceState, PawnActorStateIdCounter)
PlayerDefenceState::~PlayerDefenceState()
{

}

void PlayerDefenceState::Startup()
{
	PlayerActor* pPlayer = dynamic_cast<PlayerActor*>(GetOwner());
	ASSERT_LOG(pPlayer != nullptr);

	// 현재 무기에 따라 작동
	const std::shared_ptr<WeaponActor>& spSecondaryWeapon = pPlayer->GetWeapon(EWeaponSlotType::Secondary);
	if (spSecondaryWeapon == nullptr)
	{
		return;
	}

	spSecondaryWeapon->ApplyDynamicSpriteToOwner();
}

bool PlayerDefenceState::Update(float deltaSeconds)
{
	return (UpdateWeaponActor(deltaSeconds, GetOwner(), EWeaponSlotType::Secondary));
}

bool PlayerDefenceState::CheckKnockbackExisted() const
{
	PawnActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// 방어 상태인데 이동 벡터가 존재하면 넉백 진행중
	CellActorMoveComponent* pMoveComponent = pOwner->GetComponent<CellActorMoveComponent>();
	return (pMoveComponent->IsZeroMoveDirectionVector() == false);
}