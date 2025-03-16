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

	//// 추적 가능한 플레이어를 찾을 때만 추적 상태로 변경
	//const std::shared_ptr<PlayerActor>& spFoundPlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	//if (spFoundPlayerActor != nullptr)
	//{
	//	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
	//	DEFAULT_TRACE_LOG("몬스터 (Idle -> 추적) 상태로 전환!");
	//	return true;
	//}

	//// 그게 아니면 그냥 돌아다님
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

	// 랜덤하게 이동
	int32 randMoveDirIdx = (std::rand() % TO_NUM(EActorLookAtDirection::Count));
	const Position2d& moveDir = PawnActor::g_lookAtForwardCellPosTable[randMoveDirIdx];
	const Vector2d& vMoveDir = Vector2d{ static_cast<float>(moveDir.x), static_cast<float>(moveDir.y) };

	// 이동 정보 처리
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

#pragma region 한칸 이동하면 플레이어와 한칸 차이인지 확인
	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = spEnemyMonsterOwner->FindNearbyPlayerActor();
	if (spFoundAttackablePlayerActor != nullptr)
	{
		// 바라보는 방향 조정
		const Vector2d& vMoveDir = spEnemyMonsterOwner->CalculateMoveDirectionByCellPosition(spFoundAttackablePlayerActor->GetCellPosition());
		spEnemyMonsterOwner->ApplyMoveDirectionToLookAtDirection(vMoveDir);
		spEnemyMonsterOwner->ChangeActorStateDynamicSprite<EnmeyMonsterIdleState>();

		// Attack 상태로 전환
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterAttackState>();
		DEFAULT_TRACE_LOG("몬스터 (추적 -> 공격) 상태로 전환!");
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

#pragma region 플레이어를 추적할 수 있는지 확인
	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundChaseablePlayerActor == nullptr)
	{
		// 추적 가능한 플레이어를 못 찾았으면 Idle로 변경
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

	// 이동할 위치에서 현재 위치와의 차이로 방향 벡터 판단
	const Vector2d& vMoveDir = spEnemyMonsterOwner->CalculateMoveDirectionByCellPosition(vecNavigationPos[0]);

	// 이동 정보 처리
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

	// 바로 앞에 플레이어가 없다면 추적 상태나 Idle로 변경
	const Position2d& forwardCellPos = spEnemyMonsterOwner->CalculateForwardCellPosition();
	const std::shared_ptr<PlayerActor>& spFoundAttackablePlayerActor = pCurrentScene->FindExactTypeBaseOfCellActor<PlayerActor>(EActorLayerType::Creature, forwardCellPos);
	if (spFoundAttackablePlayerActor != nullptr)
	{
		pEnemyMonsterAttackComponent->ActivateAttackIfAttackable(spFoundAttackablePlayerActor);
		pEnemyMonsterAttackComponent->SetAttackDelayTime(1.0f);
		return false;
	}
	
	pEnemyMonsterAttackComponent->OffAttackTimer();

#pragma region 여전히 플레이어를 추적할 수 있는지 확인
	const std::shared_ptr<PlayerActor>& spFoundChaseablePlayerActor = spEnemyMonsterOwner->FindChaseAvailablePlayerActor();
	if (spFoundChaseablePlayerActor == nullptr)
	{
		spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterIdleState>();
		DEFAULT_TRACE_LOG("몬스터 (공격 -> Idle) 상태로 전환!");

		return false;
	}
#pragma endregion

	spEnemyMonsterOwner->ReserveChangeNextState<EnmeyMonsterChaseState>();
	DEFAULT_TRACE_LOG("몬스터 (공격 -> 추적) 상태로 전환!");

	return true;
}