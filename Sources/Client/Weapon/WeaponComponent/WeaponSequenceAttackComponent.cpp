// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WeaponSequenceAttackComponent.h"

#include "Client/Player/PlayerActor.h"

DEFINE_COMPILETIME_ID(WeaponSequenceAttackComponent, ComponentIdCounter)

WeaponSequenceAttackComponent::WeaponSequenceAttackComponent()
{

}

WeaponSequenceAttackComponent::~WeaponSequenceAttackComponent()
{

}

bool WeaponSequenceAttackComponent::Update(float deltaSeconds)
{
	WeaponActor* pWeapon = dynamic_cast<WeaponActor*>(GetOwner());
	ASSERT_LOG_RETURN_VALUE(pWeapon != nullptr, false);
	const std::shared_ptr<PlayerActor>& spAttacker = std::dynamic_pointer_cast<PlayerActor>(pWeapon->GetWeaponOwner());
	ASSERT_LOG_RETURN_VALUE(spAttacker != nullptr, false);

	// 이건 공격 애니메이션은 있음
	const DynamicSpriteComponent* pDynamicSpriteComponent = spAttacker->FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG_RETURN_VALUE(pDynamicSpriteComponent != nullptr, false);
	if (pDynamicSpriteComponent->CheckDynamicSpriteEnd() == false)
	{
		return false;
	}

	// 공격 애니메이션 재생이 완료된 상황

#pragma region 피해받은 액터가 있는지?
	const Position2d& forwardCellPos = spAttacker->CalculateForwardCellPosition();

	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	const PawnActorPtr& spVictim = std::dynamic_pointer_cast<PawnActor>(pCurrentScene->FindCellActor(EActorLayerType::Creature, forwardCellPos, nullptr));
	if (spVictim != nullptr)
	{
		spVictim->ProcessDamaged(spAttacker->SharedFromThisExactType<PawnActor>());
	}
#pragma endregion

	// 스프라이트는 현재 프레임에서 바로 전환
	spAttacker->ChangeActorStateDynamicSprite<PawnActorIdleState>();

	// Idle 상태로 전환
	spAttacker->ReserveChangeNextState<PawnActorIdleState>();

	DEFAULT_TRACE_LOG("(공격 -> 기본) 상태로 전환!");
	return true;
}

ComponentPtr WeaponSequenceAttackComponent::CreateClone()
{
	return std::make_shared<WeaponSequenceAttackComponent>(*this);
}