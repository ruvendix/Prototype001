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

		// ���⿡�� ���� ó��
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
	return (UpdateWeaponActor(deltaSeconds, GetOwner(), EWeaponSlotType::Secondary));
}

bool PlayerDefenceState::CheckKnockbackExisted() const
{
	PawnActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// ��� �����ε� �̵� ���Ͱ� �����ϸ� �˹� ������
	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	return (pMoveComponent->IsZeroMoveDirectionVector() == false);
}