// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WeaponSequenceAttackComponent.h"

#include "ClientSources/Player/PlayerActor.h"

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

	// �̰� ���� �ִϸ��̼��� ����
	const DynamicSpriteComponent* pDynamicSpriteComponent = spAttacker->FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG_RETURN_VALUE(pDynamicSpriteComponent != nullptr, false);
	if (pDynamicSpriteComponent->CheckDynamicSpriteEnd() == false)
	{
		return false;
	}

	/*
	���� �ִϸ��̼��� �Ϸ�Ǿ��� �� ����ü�� ��ϵǾ��ִٸ� ����ü ��� �õ�
	����ü�� ���� ���� ������ ���� ���ذ� �ִ��� Ȯ�� */
	if (pWeapon->TryCreateProjectile() == false)
	{
#pragma region ���ع��� ���Ͱ� �ִ���?
		const Position2d& forwardCellPos = spAttacker->CalculateForwardCellPosition();

		const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
		ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
		const PawnActorPtr& spVictim = std::dynamic_pointer_cast<PawnActor>(pCurrentScene->FindCellActor(EActorLayerType::Creature, forwardCellPos, nullptr));
		if (spVictim != nullptr)
		{
			spVictim->ProcessDamaged(spAttacker);
		}
#pragma endregion
	}

	// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
	spAttacker->ChangeActorStateDynamicSprite<PawnActorIdleState>();

	// Idle ���·� ��ȯ
	spAttacker->ReserveChangeNextState<PawnActorIdleState>();

	DEFAULT_TRACE_LOG("(���� -> �⺻) ���·� ��ȯ!");
	return true;
}

ComponentPtr WeaponSequenceAttackComponent::CreateClone()
{
	return std::make_shared<WeaponSequenceAttackComponent>(*this);
}