// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterAttackComponent.h"

#include "ClientSources/Player/PlayerActor.h"

DEFINE_COMPILETIME_ID(EnemyMonsterAttackComponent, ComponentIdCounter)

EnemyMonsterAttackComponent::EnemyMonsterAttackComponent()
{

}

EnemyMonsterAttackComponent::~EnemyMonsterAttackComponent()
{

}

bool EnemyMonsterAttackComponent::Update(float deltaSeconds)
{
	m_attackTimer.Update(deltaSeconds);
	return true;
}

ComponentPtr EnemyMonsterAttackComponent::CreateClone()
{
	return std::make_shared<EnemyMonsterAttackComponent>(*this);
}

void EnemyMonsterAttackComponent::ActivateAttackIfAttackable(const std::shared_ptr<PlayerActor>& spVictimPlayerActor)
{
	if (m_attackTimer.IsTimerOn())
	{
		return;
	}

	m_attackTimer = TimerManager::I()->CreateTimer(0.0f, false, this, &EnemyMonsterAttackComponent::OnAttackToVictimPlayerActor, spVictimPlayerActor);
}

void EnemyMonsterAttackComponent::OffAttackTimer()
{
	m_attackTimer.SetOff();
}

void EnemyMonsterAttackComponent::OnAttackToVictimPlayerActor(const std::shared_ptr<PlayerActor>& spVictimPlayerActor)
{
	ASSERT_LOG_RETURN(spVictimPlayerActor != nullptr);
	if (spVictimPlayerActor->IsActorFlagBitOn(EActorFlag::Activation) == false)
	{
		m_attackTimer.SetOff();
		return;
	}

	//DEFAULT_TRACE_LOG("공격 주기!");

	const std::shared_ptr<PawnActor>& spAttacker = GetOwner()->SharedFromThisExactType<PawnActor>();
	spVictimPlayerActor->ProcessDamaged(spAttacker);
}