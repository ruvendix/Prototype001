// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerState.h"

#include "Client/Player/PlayerActor.h"

namespace
{
	bool UpdateWeaponActor(float deltaSeconds, PawnActor* pOwner, EWeaponSlotType weaponSlot)
	{
		PlayerActor* pPlayer = dynamic_cast<PlayerActor*>(pOwner);
		ASSERT_LOG_RETURN_VALUE(pPlayer != nullptr, false);

		// 무기에서 전부 처리
		const std::shared_ptr<WeaponActor>& spWeaponActor = pPlayer->GetWeaponActor(weaponSlot);
		return spWeaponActor->Update(deltaSeconds);
	}
}

DEFINE_COMPILETIME_ID(PlayerAttackState, PawnActorStateIdCounter)
PlayerAttackState::~PlayerAttackState()
{

}

void PlayerAttackState::Startup()
{

}

bool PlayerAttackState::Update(float deltaSeconds)
{
	return (UpdateWeaponActor(deltaSeconds, GetOwner(), EWeaponSlotType::Primary));
}

DEFINE_COMPILETIME_ID(PlayerDefenceState, PawnActorStateIdCounter)
PlayerDefenceState::~PlayerDefenceState()
{

}

void PlayerDefenceState::Startup()
{

}

bool PlayerDefenceState::Update(float deltaSeconds)
{
	PawnActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	if ((CheckKnockbackExisted() == true) &&
		(pMoveComponent->TryCheckValidateGoalPosition(deltaSeconds, true) == true))
	{
		pMoveComponent->ResetMoveDirectionVector();
		pMoveComponent->ApplyDestinationDataToOwner();

		DEFAULT_TRACE_LOG("방어 상태에서 넉백!");
	}

	return (UpdateWeaponActor(deltaSeconds, GetOwner(), EWeaponSlotType::Secondary));
}

bool PlayerDefenceState::CheckKnockbackExisted() const
{
	PawnActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// 방어 상태인데 이동 벡터가 존재하면 넉백 진행중
	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	return (pMoveComponent->IsZeroMoveDirectionVector() == false);
}