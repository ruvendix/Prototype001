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
	//EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
	//if (spEnemyMonsterOwner == nullptr)
	//{
	//	return false;
	//}

	//// ���� ������ �÷��̾ ã�� ���� ���� ���·� ����
	//const std::shared_ptr<PlayerActor>& spFoundPlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	//if (spFoundPlayerActor != nullptr)
	//{
	//	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
	//	DEFAULT_TRACE_LOG("���� (Idle -> ����) ���·� ��ȯ!");
	//	return true;
	//}

	//// �װ� �ƴϸ� �׳� ���ƴٴ�
	//CellActorMoveComponent* pCellActorMoveComponent = spEnemyMonsterOwner->FindComponent<CellActorMoveComponent>();
	//ASSERT_LOG_RETURN_VALUE(pCellActorMoveComponent != nullptr, false);
	//if (pCellActorMoveComponent->IsZeroMoveDirectionVector())
	//{
	//	ProcessIdleStateByRandomMove();
	//}

	return true;
}

void EnmeyMonsterIdleState::ProcessIdleStateByRandomMove() const
{
	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
	if (spEnemyMonsterOwner == nullptr)
	{
		return;
	}

	// �����ϰ� �̵�
	int32 randMoveDirIdx = (std::rand() % TO_NUM(EActorLookAtDirection::Count));
	const Position2d& moveDir = PawnActor::g_lookAtForwardCellPosTable[randMoveDirIdx];
	const Vector2d& vMoveDir = Vector2d{ static_cast<float>(moveDir.x), static_cast<float>(moveDir.y) };

	// �̵� ���� ó��
	CellActorMoveComponent* pCellActorMoveComponent = spEnemyMonsterOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pCellActorMoveComponent != nullptr);
	if (pCellActorMoveComponent->ProcessMoveDirection(vMoveDir, true) == false)
	{
		return;
	}

	spEnemyMonsterOwner->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();
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
	PawnActor* pOwner = GetOwner();
	ASSERT_LOG(pOwner != nullptr);

	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(pOwner);
	if (spEnemyMonsterOwner == nullptr)
	{
		return false;
	}

	CellActorMoveComponent* pCellActorMoveComponent = spEnemyMonsterOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pCellActorMoveComponent != nullptr, false);
	if (pCellActorMoveComponent->IsZeroMoveDirectionVector() == false)
	{
		return false;
	}

#pragma region ��ĭ �̵��ϸ� �÷��̾�� ��ĭ �������� Ȯ��
	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = spEnemyMonsterOwner->FindNearbyPlayerActor();
	if (spFoundAttackablePlayerActor != nullptr)
	{
		// �ٶ󺸴� ���� ����
		const Vector2d& vMoveDir = spEnemyMonsterOwner->CalculateMoveDirectionByCellPosition(spFoundAttackablePlayerActor->GetCellPosition());
		spEnemyMonsterOwner->ApplyMoveDirectionToLookAtDirection(vMoveDir);
		spEnemyMonsterOwner->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();

		// Attack ���·� ��ȯ
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterAttackState>();
		DEFAULT_TRACE_LOG("���� (���� -> ����) ���·� ��ȯ!");
		return true;
	}
#pragma endregion

	ProcessChaseStateByNavigationPath();
	return true;
}

void EnmeyMonsterChaseState::ProcessChaseStateByNavigationPath() const
{
	EnemyMonsterActor* spEnemyMonsterOwner = dynamic_cast<EnemyMonsterActor*>(GetOwner());
	if (spEnemyMonsterOwner == nullptr)
	{
		return;
	}

	CellActorMoveComponent* pCellActorMoveComponent = spEnemyMonsterOwner->FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pCellActorMoveComponent != nullptr);

#pragma region �÷��̾ ������ �� �ִ��� Ȯ��
	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundChaseablePlayerActor == nullptr)
	{
		// ���� ������ �÷��̾ �� ã������ Idle�� ����
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return;
	}
#pragma endregion

	std::vector<Position2d> vecNavigationPos;
	EnemyMonsterActor::CalculateNavigationPath(spEnemyMonsterOwner->GetCellPosition(),
		spFoundChaseablePlayerActor->GetCellPosition(), spEnemyMonsterOwner->GetChaseRange(), vecNavigationPos);

	if (vecNavigationPos.empty() == true)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return;
	}

	// �̵��� ��ġ���� ���� ��ġ���� ���̷� ���� ���� �Ǵ�
	const Vector2d& vMoveDir = spEnemyMonsterOwner->CalculateMoveDirectionByCellPosition(vecNavigationPos[0]);

	// �̵� ���� ó��
	if (pCellActorMoveComponent->ProcessMoveDirection(vMoveDir, true) == false)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		return;
	}

	spEnemyMonsterOwner->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();
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