// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterState.h"

#include "ClientSources/Player/PlayerActor.h"
#include "ClientSources/EnemyMonster/EnemyMonsterComponent/EnemyMonsterAttackComponent.h"

DEFINE_COMPILETIME_ID(EnmeyMonsterIdleState, PawnActorStateIdCounter)
EnmeyMonsterIdleState::~EnmeyMonsterIdleState()
{

}

void EnmeyMonsterIdleState::Startup()
{

}

bool EnmeyMonsterIdleState::Update(float deltaSeconds)
{
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterChaseState, PawnActorStateIdCounter)
EnmeyMonsterChaseState::~EnmeyMonsterChaseState()
{

}

void EnmeyMonsterChaseState::Startup()
{

}

bool EnmeyMonsterChaseState::Update(float deltaSeconds)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterAttackState, PawnActorStateIdCounter)
EnmeyMonsterAttackState::~EnmeyMonsterAttackState()
{

}

void EnmeyMonsterAttackState::Startup()
{

}

bool EnmeyMonsterAttackState::Update(float deltaSeconds)
{
//	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
//	if (spEnemyMonsterOwner == nullptr)
//	{
//		return false;
//	}
//
//	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
//	ASSERT_LOG(pCurrentScene != nullptr);
//
//	EnemyMonsterAttackComponent* pEnemyMonsterAttackComponent = spEnemyMonsterOwner->FindComponent<EnemyMonsterAttackComponent>();
//	ASSERT_LOG_RETURN_VALUE(pEnemyMonsterAttackComponent != nullptr, false);
//
//	// �ٷ� �տ� �÷��̾ ���ٸ� ���� ���³� Idle�� ����
//	const Position2d& forwardCellPos = spEnemyMonsterOwner->CalculateForwardCellPosition();
//	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = pCurrentScene->FindExactTypeBaseOfCellActor<PlayerActor>(EActorLayerType::Creature, forwardCellPos);
//	if (spFoundAttackablePlayerActor != nullptr)
//	{
//		pEnemyMonsterAttackComponent->ActivateAttackIfAttackable(spFoundAttackablePlayerActor);
//		pEnemyMonsterAttackComponent->SetAttackDelayTime(1.0f);
//		return false;
//	}
//	
//	pEnemyMonsterAttackComponent->OffAttackTimer();
//
//#pragma region ������ �÷��̾ ������ �� �ִ��� Ȯ��
//	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
//	if (spFoundChaseablePlayerActor == nullptr)
//	{
//		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
//		DEFAULT_TRACE_LOG("���� (���� -> Idle) ���·� ��ȯ!");
//
//		return false;
//	}
//#pragma endregion
//
//	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
//	DEFAULT_TRACE_LOG("���� (���� -> ����) ���·� ��ȯ!");

	return true;
}