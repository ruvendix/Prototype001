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

	// ��ĭ �̵��ߴٸ� ��� �̵�������
	CellActorMoveComponent* pMoveComponent = pAttacker->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pMoveComponent != nullptr, false);
	if (pMoveComponent->IsZeroMoveDirectionVector())
	{
		pMoveComponent->ProcessMoving();
	}

	const PawnActorPtr& spAttacker = pAttacker->SharedFromThisExactType<PawnActor>();
	const Position2d& attackerCellPos = pAttacker->GetCellPosition();

#pragma region ���ع��� ���Ͱ� �ִ���?
	Scene * pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);

	const PawnActorPtr& spVictim = std::dynamic_pointer_cast<PawnActor>(pCurrentScene->FindCellActor(EActorLayerType::Creature, attackerCellPos, nullptr));
	if (spVictim != nullptr)
	{
		spVictim->ProcessDamaged(spAttacker);
		pAttacker->ProcessDamaged(spAttacker);
		DEFAULT_TRACE_LOG("����ü�� �����ؼ� �����!");
		return false;
	}
#pragma endregion

#pragma region ���� ������?
	if (pCurrentScene->CheckCanMoveToCellPosition(attackerCellPos, nullptr) == false)
	{
		pAttacker->ProcessDamaged(spAttacker);
		DEFAULT_TRACE_LOG("����ü�� ������ �����!");
		return false;
	}
#pragma endregion

	return true;
}

ComponentPtr ProjectileAttackComponent::CreateClone()
{
	return std::make_shared<ProjectileAttackComponent>(*this);
}