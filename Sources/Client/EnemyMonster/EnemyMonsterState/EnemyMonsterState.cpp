// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterState.h"

#include "Client/Player/PlayerActor.h"
#include "Client/EnemyMonster/EnemyMonsterComponent/EnemyMonsterAttackComponent.h"

DEFINE_COMPILETIME_ID(EnmeyMonsterIdleState, AnimationActorStateIdCounter)
EnmeyMonsterIdleState::~EnmeyMonsterIdleState()
{
}

void EnmeyMonsterIdleState::Startup()
{

}

bool EnmeyMonsterIdleState::Update(float deltaSeconds)
{
	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
	if (spEnemyMonsterOwner == nullptr)
	{
		return false;
	}

	// ���� ������ �÷��̾ ã�� ���� ���� ���·� ����
	const std::shared_ptr<PlayerActor>& spFoundPlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundPlayerActor == nullptr)
	{
		return false;
	}

	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
	DEFAULT_TRACE_LOG("���� (Idle -> ����) ���·� ��ȯ!");

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterChaseState, AnimationActorStateIdCounter)
EnmeyMonsterChaseState::~EnmeyMonsterChaseState()
{

}

void EnmeyMonsterChaseState::Startup()
{

}

bool EnmeyMonsterChaseState::Update(float deltaSeconds)
{
	AnimationActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	// �̵� ������Ʈ
	CellActorMoveComponent* pMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pMoveComponent != nullptr, false);
	if (pMoveComponent->CheckGoalPosition(deltaSeconds) == false)
	{
		return false;
	}

	pMoveComponent->ResetMoveDirection();

	// �̵��� �Ϸ�Ǿ����� ������ �� ��ǥ�� ���� �� ��ǥ�� ����
	pMoveComponent->ApplyDestinationDataToOwner();

	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(pOwner);
	if (spEnemyMonsterOwner == nullptr)
	{
		return false;
	}

#pragma region ��ĭ �̵��ϸ� �÷��̾�� ��ĭ �������� Ȯ��
	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = spEnemyMonsterOwner->FindNearbyPlayerActor();
	if (spFoundAttackablePlayerActor != nullptr)
	{
		// �ٶ󺸴� ���� ����
		const Vec2d& vMoveDir = spEnemyMonsterOwner->CalculateMoveDirectionByCellPosition(spFoundAttackablePlayerActor->GetCellPosition());
		spEnemyMonsterOwner->ApplyMoveDirectionToLookAtDirection(vMoveDir);
		spEnemyMonsterOwner->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();

		// Attack ���·� ��ȯ
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterAttackState>();
		DEFAULT_TRACE_LOG("���� (���� -> ����) ���·� ��ȯ!");
		return true;
	}
#pragma endregion

#pragma region ��ĭ �̵��ϸ� ������ �÷��̾ ������ �� �ִ��� Ȯ��
	// ���� ������ �÷��̾ �� ã������ Idle�� ����
	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundChaseablePlayerActor == nullptr)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return false;
	}
#pragma endregion

	std::vector<Position2d> vecNavigationPos;
	EnemyMonsterActor::CalculateNavigationPath(spEnemyMonsterOwner->GetCellPosition(),
		spFoundChaseablePlayerActor->GetCellPosition(), spEnemyMonsterOwner->GetChaseRange(), vecNavigationPos);

	if (vecNavigationPos.empty() == true)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return false;
	}

	// �̵��� ��ġ���� ���� ��ġ���� ���̷� ���� ���� �Ǵ�
	const Vec2d& vMoveDir = pOwner->CalculateMoveDirectionByCellPosition(vecNavigationPos[0]);

	// �̵� ó��
	CellActorMoveComponent* pCellActorMoveComponent = pOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pCellActorMoveComponent != nullptr, false);
	if (pCellActorMoveComponent->ProcessMove(vMoveDir) == false)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return false;
	}

	GetOwner()->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DEFINE_COMPILETIME_ID(EnmeyMonsterAttackState, AnimationActorStateIdCounter)
EnmeyMonsterAttackState::~EnmeyMonsterAttackState()
{

}

void EnmeyMonsterAttackState::Startup()
{

}

bool EnmeyMonsterAttackState::Update(float deltaSeconds)
{
	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
	if (spEnemyMonsterOwner == nullptr)
	{
		return false;
	}

	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG(pCurrentScene != nullptr);

	EnemyMonsterAttackComponent* pEnemyMonsterAttackComponent = spEnemyMonsterOwner->FindComponent<EnemyMonsterAttackComponent>();
	ASSERT_LOG_RETURN_VALUE(pEnemyMonsterAttackComponent != nullptr, false);

	// �ٷ� �տ� �÷��̾ ���ٸ� ���� ���³� Idle�� ����
	const Position2d& forwardCellPos = spEnemyMonsterOwner->CalculateForwardCellPosition();
	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = pCurrentScene->FindExactTypeBaseOfCellActor<PlayerActor>(EActorLayerType::Creature, forwardCellPos);
	if (spFoundAttackablePlayerActor != nullptr)
	{
		pEnemyMonsterAttackComponent->ActivateAttackIfAttackable(spFoundAttackablePlayerActor);
		pEnemyMonsterAttackComponent->SetAttackDelayTime(1.0f);
		return false;
	}
	
	pEnemyMonsterAttackComponent->OffAttackTimer();

#pragma region ������ �÷��̾ ������ �� �ִ��� Ȯ��
	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundChaseablePlayerActor == nullptr)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		DEFAULT_TRACE_LOG("���� (���� -> Idle) ���·� ��ȯ!");

		return false;
	}
#pragma endregion

	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
	DEFAULT_TRACE_LOG("���� (���� -> ����) ���·� ��ȯ!");

	return true;
}