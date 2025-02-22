// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ProjectileAttackComponent.h"

DEFINE_COMPILETIME_ID(ProjectileAttackComponent, ComponentIdCounter)

ProjectileAttackComponent::ProjectileAttackComponent()
{

}

ProjectileAttackComponent::~ProjectileAttackComponent()
{

}

bool ProjectileAttackComponent::Update(float deltaSeconds)
{
	ProjectileActor* pAttacker = dynamic_cast<ProjectileActor*>(GetOwner());
	ASSERT_LOG_RETURN_VALUE(pAttacker != nullptr, false);

	// 한칸 이동했다면 계속 이동시켜줌
	CellActorMoveComponent* pMoveComponent = pAttacker->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pMoveComponent != nullptr, false);
	if (pMoveComponent->IsZeroMoveDirectionVector())
	{
		pMoveComponent->ProcessMoving();
	}

	const PawnActorPtr& spAttacker = pAttacker->SharedFromThisExactType<PawnActor>();
	const Position2d& attackerCellPos = pAttacker->GetCellPosition();

#pragma region 피해받은 액터가 있는지?
	Scene * pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);

	const PawnActorPtr& spVictim = std::dynamic_pointer_cast<PawnActor>(pCurrentScene->FindCellActor(EActorLayerType::Creature, attackerCellPos, nullptr));
	if (spVictim != nullptr)
	{
		spVictim->ProcessDamaged(spAttacker);
		pAttacker->ProcessDamaged(spAttacker);
		DEFAULT_TRACE_LOG("투사체가 공격해서 사라짐!");
		return false;
	}
#pragma endregion

#pragma region 막힌 길인지?
	if (pCurrentScene->CheckCanMoveToCellPosition(attackerCellPos, nullptr) == false)
	{
		pAttacker->ProcessDamaged(spAttacker);
		DEFAULT_TRACE_LOG("투사체가 막혀서 사라짐!");
		return false;
	}
#pragma endregion

	return true;
}

ComponentPtr ProjectileAttackComponent::CreateClone()
{
	return std::make_shared<ProjectileAttackComponent>(*this);
}