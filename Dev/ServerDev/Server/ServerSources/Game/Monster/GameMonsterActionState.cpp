// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonsterActionState.h"

#include "GameMonsterUtility.h"

GameMonsterActionState::GameMonsterActionState(const std::shared_ptr<GameMonster>& spOwner)
{
	m_spOwner = spOwner;
}

GameMonsterActionState::~GameMonsterActionState()
{

}

void GameMonsterActionState::Startup()
{
	ResetActionTimer();
	m_actionTimer = TimerManager::I()->CreateTimer(0.0f, true, this, &GameMonsterActionState::OnActionTimer);
}

bool GameMonsterActionState::Update(float deltaSeconds)
{
	m_actionTimer.Update(deltaSeconds);
	return true;
}

std::shared_ptr<GameMonster> GameMonsterActionState::BringOwner() const
{
	if (m_spOwner.expired() == true)
	{
		return nullptr;
	}

	return (m_spOwner.lock());
}

void GameMonsterActionState::ModifyActionTriggerTime(float actionTriggerTime)
{
	m_actionTimer.SetTriggerTime(actionTriggerTime);
}

void GameMonsterActionState::ResetActionTimer()
{
	m_actionTimer.Reset();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameMonsterIdleState::Startup()
{
	Super::Startup();
	ModifyActionTriggerTime(1.0f);
}

void GameMonsterIdleState::OnActionTimer()
{
	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN(spOwner != nullptr);
	Protocol::GameEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();

	// 가장 가까운 유저를 찾아봄
	const GamePlayerPtr& spFoundPlayer = GameMonsterUtility::FindNearestPlayer(spOwner, spOwner->GetChaseRange());
	if (spFoundPlayer != nullptr)
	{
		// 찾은 유저와의 거리가 공격 거리라면 공격 상태로 전환
		const Position2d& spMonsterCurrentCellPos = spOwner->MakeCurrentCellPosition();
		const Position2d& spFoundPlayerCellPos = spFoundPlayer->MakeCurrentCellPosition();
		if (GameMonsterUtility::CheckValidDistance(spMonsterCurrentCellPos, spFoundPlayerCellPos, spOwner->GetAttackableRange()) == true)
		{
			refGameEntityInfo.set_entity_state(Protocol::EGameEntityState::Attack);
			spOwner->ReserveNextState<GameMonsterAttackState>();
			DEFAULT_TRACE_LOG("몬스터 (Idle -> Attack) 전환!");
		}
		else
		{
			refGameEntityInfo.set_entity_state(Protocol::EGameEntityState::Chase);
			spOwner->ReserveNextState<GameMonsterChaseState>();
			DEFAULT_TRACE_LOG("몬스터 (Idle -> Chase) 전환!");
		}

		const RxSendBufferPtr& syncGameEntityStatePacket = RxServerPacketHandler::I()->MakeSyncGameEntityStatePacket(refGameEntityInfo);
		RxGameSessionManager::I()->Broadcast(syncGameEntityStatePacket);
	}
	else
	{
		ProcessRandomMove();
	}
}

void GameMonsterIdleState::ProcessRandomMove()
{
	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN(spOwner != nullptr);
	Protocol::GameEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();

	// 이동 가능한 곳을 못찾았으면 다음 기회에
	int32 randMoveDirIdx = (std::rand() % TO_NUM(Protocol::EGameEntityLookAtDir::Count));
	const Position2d& moveDir = PawnActor::g_lookAtForwardCellPosTable[randMoveDirIdx];

	Position2d destCellPos =
	{
		static_cast<int32>(refGameEntityInfo.cell_pos_x()) + moveDir.x,
		static_cast<int32>(refGameEntityInfo.cell_pos_y()) + moveDir.y
	};

	if (GameRoom::I()->CheckCanMoveToCellPosition(destCellPos, nullptr) == false)
	{
		return;
	}

	refGameEntityInfo.set_cell_pos_x(destCellPos.x);
	refGameEntityInfo.set_cell_pos_y(destCellPos.y);
	refGameEntityInfo.set_entity_state(Protocol::EGameEntityState::Walk);
	refGameEntityInfo.set_entitye_look_at_dir(static_cast<Protocol::EGameEntityLookAtDir>(randMoveDirIdx));

	const RxSendBufferPtr& syncGameEntityMovePacket = RxServerPacketHandler::I()->MakeSyncGameEntityMovePacket(refGameEntityInfo);
	RxGameSessionManager::I()->Broadcast(syncGameEntityMovePacket);

	spOwner->ReserveNextState<GameMonsterWalkState>();
	DEFAULT_TRACE_LOG("몬스터 (Idle -> Walk) 전환!");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameMonsterWalkState::Startup()
{
	Super::Startup();
	ModifyActionTriggerTime(1.0f);
}

void GameMonsterWalkState::OnActionTimer()
{
	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN(spOwner != nullptr);

	Protocol::GameEntityInfo gameEntityInfo = spOwner->GetGameEntityInfo();
	gameEntityInfo.set_entity_state(Protocol::EGameEntityState::Idle);

	const RxSendBufferPtr& syncGameEntityStatePacket = RxServerPacketHandler::I()->MakeSyncGameEntityStatePacket(gameEntityInfo);
	RxGameSessionManager::I()->Broadcast(syncGameEntityStatePacket);

	spOwner->ReserveNextState<GameMonsterIdleState>();
	DEFAULT_TRACE_LOG("몬스터 (Walk -> Idle) 전환!");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameMonsterChaseState::Startup()
{
	Super::Startup();
	ModifyActionTriggerTime(3.0f);
}

void GameMonsterChaseState::OnActionTimer()
{
	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN(spOwner != nullptr);
	Protocol::GameEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();

	// 길부터 찾아보고 만약 길 찾기에 실패하면 Idle로 전환
	std::vector<Position2d> vecNavigationPath;
	if (ProcessNavigationPath(vecNavigationPath) == false)
	{
		// 유저를 공격할 수 있는 거리인지 확인
		const GamePlayerPtr& spFoundPlayer = GameMonsterUtility::FindNearestPlayer(spOwner, 1);
		if (spFoundPlayer != nullptr)
		{
			refGameEntityInfo.set_entity_state(Protocol::EGameEntityState::Attack);
			spOwner->ReserveNextState<GameMonsterAttackState>();
			DEFAULT_TRACE_LOG("몬스터 (Chase -> Attack) 전환!");
		}
		else
		{
			refGameEntityInfo.set_entity_state(Protocol::EGameEntityState::Idle);
			spOwner->ReserveNextState<GameMonsterIdleState>();
			DEFAULT_TRACE_LOG("몬스터 (Chase -> Idle) 전환!");
		}

		const RxSendBufferPtr& syncGameEntityStatePacket = RxServerPacketHandler::I()->MakeSyncGameEntityStatePacket(refGameEntityInfo);
		RxGameSessionManager::I()->Broadcast(syncGameEntityStatePacket);
	}
	else
	{
		ProcessNavigationMove(vecNavigationPath[0]);
	}
}

bool GameMonsterChaseState::ProcessNavigationPath(std::vector<Position2d>& outVecNavigationPos) const
{
	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN_VALUE(spOwner != nullptr, false);
	Protocol::GameEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();

	// 유저를 추적할 수 있는지 확인
	const GamePlayerPtr& spFoundPlayer = GameMonsterUtility::FindNearestPlayer(spOwner, spOwner->GetChaseRange());
	if (spFoundPlayer == nullptr)
	{
		return false;
	}

	const Position2d& monsterCellPos = spOwner->MakeCurrentCellPosition();
	const Position2d& foundPlayerCellPos = spFoundPlayer->MakeCurrentCellPosition();

	GameMonsterUtility::CalculateNavigationPath(monsterCellPos, foundPlayerCellPos, spOwner->GetChaseRange(), outVecNavigationPos);
	if (outVecNavigationPos.empty() == true)
	{
		return false;
	}

	return true;
}

void GameMonsterChaseState::ProcessNavigationMove(const Position2d& moveNavigationPos) const
{
	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN(spOwner != nullptr);

	// 이동할 좌표로 바라보는 방향 알아내기
	Protocol::EGameEntityLookAtDir lookAtDir = spOwner->CalculateGameEntityLookAtDirection(moveNavigationPos);

	Protocol::GameEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();
	refGameEntityInfo.set_entitye_look_at_dir(lookAtDir);

	// 바라보는 방향 변경 후 이동할 좌표 적용
	refGameEntityInfo.set_cell_pos_x(moveNavigationPos.x);
	refGameEntityInfo.set_cell_pos_y(moveNavigationPos.y);

	const RxSendBufferPtr& syncGameEntityMovePacket = RxServerPacketHandler::I()->MakeSyncGameEntityMovePacket(refGameEntityInfo);
	RxGameSessionManager::I()->Broadcast(syncGameEntityMovePacket);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameMonsterAttackState::Startup()
{
	Super::Startup();
	ModifyActionTriggerTime(1.0f);
}

void GameMonsterAttackState::OnActionTimer()
{
	DEFAULT_TRACE_LOG("공격 개시!");
}