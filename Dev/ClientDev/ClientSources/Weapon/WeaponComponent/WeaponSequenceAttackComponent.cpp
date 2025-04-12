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

	const PlayerActorPtr& spAttacker = std::dynamic_pointer_cast<PlayerActor>(pWeapon->GetWeaponOwner());
	ASSERT_LOG_RETURN_VALUE(spAttacker != nullptr, false);

	bool bUpdateResult = false;
	if (spAttacker->IsLocalPlayer()) // ���� �÷��̾ ������Ʈ
	{
		bUpdateResult = UpdateForLocal(deltaSeconds);
	}

	DEFAULT_TRACE_LOG("(���� -> �⺻) ���·� ��ȯ!");
	return true;
}

ComponentPtr WeaponSequenceAttackComponent::CreateClone()
{
	return std::make_shared<WeaponSequenceAttackComponent>(*this);
}

bool WeaponSequenceAttackComponent::UpdateForLocal(float deltaSeconds)
{
	WeaponActor* pWeapon = dynamic_cast<WeaponActor*>(GetOwner());
	ASSERT_LOG_RETURN_VALUE(pWeapon != nullptr, false);

	const ActionableActorPtr& spAttacker = pWeapon->GetWeaponOwner();
	ASSERT_LOG_RETURN_VALUE(spAttacker != nullptr, false);

	// ���� �ִϸ��̼��� ����
	const DynamicSpriteComponent* pDynamicSpriteComponent = spAttacker->GetComponent<DynamicSpriteComponent>();
	ASSERT_LOG_RETURN_VALUE(pDynamicSpriteComponent != nullptr, false);
	if (pDynamicSpriteComponent->CheckDynamicSpriteEnd() == false)
	{
		return false;
	}

	// �Ʒ� �κ��� ���ð� ��Ʈ��ũ�� �ٸ�!
	const WeaponActorPtr& spCurrentWeapon = spAttacker->GetCurrentWeapon();
	ASSERT_LOG(spCurrentWeapon != nullptr);

	/*
	���� �ִϸ��̼��� �Ϸ�Ǿ��� �� ����ü�� ��ϵǾ��ִٸ� ����ü ��� �õ�
	����ü�� ���� ���� ������ ���� ���ذ� �ִ��� Ȯ�� */
	if (spCurrentWeapon->TryCreateProjectile() == false)
	{
#pragma region ���ع��� ���Ͱ� �ִ���?
		const Position2d& forwardCellPos = spAttacker->CalculateForwardCellPosition();

		const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
		ASSERT_LOG(pCurrentScene != nullptr);
		const CommunicationActorPtr& spVictim = std::dynamic_pointer_cast<CommunicationActor>(pCurrentScene->FindCellActor(EActorLayerType::Creature, forwardCellPos, nullptr));
		if (spVictim != nullptr)
		{
			const RxSendBufferPtr& spSendAttckToEntityPacket = ClientPacketHandler::I()->MakeAttckToEntityPacket(spAttacker->GetEntityInfo(), spVictim->GetEntityInfo());
			NetworkManager::I()->SendPacket(spSendAttckToEntityPacket);
		}
#pragma endregion
	}

	// ��������Ʈ�� ���� �����ӿ��� �ٷ� ��ȯ
	spAttacker->ChangeActorStateDynamicSprite<PawnActorIdleState>();

	// Idle ���·� ��ȯ
	spAttacker->ReserveChangeNextState<PawnActorIdleState>();
	return true;
}