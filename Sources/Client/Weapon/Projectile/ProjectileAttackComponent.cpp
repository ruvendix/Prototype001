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
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);

	ProjectileActor* pAttacker = dynamic_cast<ProjectileActor*>(GetOwner());
	ASSERT_LOG_RETURN_VALUE(pAttacker != nullptr, false);

#pragma region ���� ������?
	if (pCurrentScene->CheckCanMoveToCellPosition(pAttacker->GetCellPosition(), nullptr) == false)
	{
		const ActorPtr& spAttacker = pAttacker->SharedFromThisExactType<Actor>();
		pCurrentScene->ReserveEraseActor(spAttacker);
		DEFAULT_TRACE_LOG("����ü�� ������ �����!");
		return false;
	}
#pragma endregion

#pragma region ���ع��� ���Ͱ� �ִ���?
	const Position2d& attackerCellPos = pAttacker->GetCellPosition();
	const PawnActorPtr& spVictim = std::dynamic_pointer_cast<PawnActor>(pCurrentScene->FindCellActor(EActorLayerType::Creature, attackerCellPos, nullptr));
	if (spVictim != nullptr)
	{
		const PawnActorPtr& spAttacker = pAttacker->SharedFromThisExactType<PawnActor>();
		spVictim->ProcessDamaged(spAttacker);
		pCurrentScene->ReserveEraseActor(spAttacker);
		DEFAULT_TRACE_LOG("����ü�� �����ؼ� �����!");
		return false;
	}
#pragma endregion
	return true;
}

ComponentPtr ProjectileAttackComponent::CreateClone()
{
	return std::make_shared<ProjectileAttackComponent>(*this);
}