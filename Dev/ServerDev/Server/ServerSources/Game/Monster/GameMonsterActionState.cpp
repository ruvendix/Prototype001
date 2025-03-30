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
	Protocol::NetworkEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();

	// 가장 가까운 유저를 찾아봄
	const GamePlayerPtr& spFoundPlayer = GameMonsterUtility::FindNearestPlayer(spOwner, spOwner->GetChaseRange());
	if (spFoundPlayer != nullptr)
	{
		// 찾은 유저와의 거리가 공격 거리라면 공격 상태로 전환
		const Position2d& spMonsterCurrentCellPos = spOwner->MakeCurrentCellPosition();
		const Position2d& spFoundPlayerCellPos = spFoundPlayer->MakeCurrentCellPosition();
		if (GameMonsterUtility::CheckValidDistance(spMonsterCurrentCellPos, spFoundPlayerCellPos, spOwner->GetAttackableRange()) == true)
		{
			refGameEntityInfo.set_entity_state(Protocol::ENetworkEntityState::Attack);
			const std::shared_ptr<GameMonsterAttackState>& spAttackState = spOwner->ReserveNextState<GameMonsterAttackState>();
			spAttackState->SetVictimPlayer(spFoundPlayer);
			DEFAULT_TRACE_LOG("몬스터 (Idle -> Attack) 전환!");
		}
		else
		{
			refGameEntityInfo.set_entity_state(Protocol::ENetworkEntityState::Chase);
			spOwner->ReserveNextState<GameMonsterChaseState>();
			DEFAULT_TRACE_LOG("몬스터 (Idle -> Chase) 전환!");
		}

		const RxSendBufferPtr& syncGameEntityStatePacket = RxServerPacketHandler::I()->MakeModifyEntityStatePacket(refGameEntityInfo);
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
	Protocol::NetworkEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();

	// 이동 가능한 곳을 못찾았으면 다음 기회에
	int32 randMoveDirIdx = (std::rand() % Protocol::ENetworkEntityLookAtDirection_ARRAYSIZE);
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
	refGameEntityInfo.set_entity_state(Protocol::ENetworkEntityState::Walk);
	refGameEntityInfo.set_entitye_look_at_dir(static_cast<Protocol::ENetworkEntityLookAtDirection>(randMoveDirIdx));

	const RxSendBufferPtr& syncGameEntityMovePacket = RxServerPacketHandler::I()->MakeMoveEntityPacket(refGameEntityInfo);
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

	Protocol::NetworkEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();
	refGameEntityInfo.set_entity_state(Protocol::ENetworkEntityState::Idle);

	const RxSendBufferPtr& syncGameEntityStatePacket = RxServerPacketHandler::I()->MakeModifyEntityStatePacket(refGameEntityInfo);
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
	Protocol::NetworkEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();

	// 길부터 찾아보고 만약 길 찾기에 실패하면 Idle로 전환
	std::vector<Position2d> vecNavigationPath;
	if (ProcessNavigationPath(vecNavigationPath) == false)
	{
		// 유저를 공격할 수 있는 거리인지 확인
		const GamePlayerPtr& spFoundPlayer = GameMonsterUtility::FindNearestPlayer(spOwner, 1);
		if (spFoundPlayer != nullptr)
		{
			refGameEntityInfo.set_entity_state(Protocol::ENetworkEntityState::Attack);
			const std::shared_ptr<GameMonsterAttackState>& spAttackState = spOwner->ReserveNextState<GameMonsterAttackState>();
			spAttackState->SetVictimPlayer(spFoundPlayer);
			DEFAULT_TRACE_LOG("몬스터 (Chase -> Attack) 전환!");
		}
		else
		{
			refGameEntityInfo.set_entity_state(Protocol::ENetworkEntityState::Idle);
			spOwner->ReserveNextState<GameMonsterIdleState>();
			DEFAULT_TRACE_LOG("몬스터 (Chase -> Idle) 전환!");
		}

		const RxSendBufferPtr& syncGameEntityStatePacket = RxServerPacketHandler::I()->MakeModifyEntityStatePacket(refGameEntityInfo);
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

	// 유저를 추격할 수 있는지 확인
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
	Protocol::ENetworkEntityLookAtDirection lookAtDir = spOwner->CalculateGameEntityLookAtDirection(moveNavigationPos);

	Protocol::NetworkEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();
	refGameEntityInfo.set_entitye_look_at_dir(lookAtDir);

	// 바라보는 방향 변경 후 이동할 좌표 적용
	refGameEntityInfo.set_cell_pos_x(moveNavigationPos.x);
	refGameEntityInfo.set_cell_pos_y(moveNavigationPos.y);

	const RxSendBufferPtr& syncGameEntityMovePacket = RxServerPacketHandler::I()->MakeMoveEntityPacket(refGameEntityInfo);
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
	if (m_spVictimPlayer == nullptr)
	{
		ToIdleState();
		return;
	}

	Protocol::NetworkEntityInfo& refVictimPlayerInfo = m_spVictimPlayer->GetGameEntityInfo();
	const Position2d& victimCellPos = m_spVictimPlayer->MakeCurrentCellPosition();

	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN(spOwner != nullptr);

	// 공격 방향 알아내기
	Protocol::ENetworkEntityLookAtDirection attackerLookAtDir = spOwner->CalculateGameEntityLookAtDirection(victimCellPos);
	Protocol::NetworkEntityInfo& refAttackerInfo = spOwner->GetGameEntityInfo();
	refAttackerInfo.set_entitye_look_at_dir(attackerLookAtDir);

	// 스탯 계산인데 일단 임시
	int32 remainVictimPlayerHp = ((refVictimPlayerInfo.hp() + refVictimPlayerInfo.defense()) - refAttackerInfo.attack());
	remainVictimPlayerHp = global::Clamp(remainVictimPlayerHp, 0, refVictimPlayerInfo.max_hp());
	refVictimPlayerInfo.set_hp(remainVictimPlayerHp);

	bool bPlayerDeath = (remainVictimPlayerHp <= 0);
	if (bPlayerDeath == true) // 사망
	{
		m_spVictimPlayer = nullptr;
		refVictimPlayerInfo.set_entity_state(Protocol::ENetworkEntityState::Death);

		ToIdleState();
	}
	else // 공격자와 멀어진 경우는 상황에 따라 판단
	{
		// 전방에 있는지?
		const Position2d& attackerForwardCellPos = spOwner->MakeForwardCellPosition();
		if (attackerForwardCellPos != victimCellPos)
		{
			// 추격 상태로 변경
			refAttackerInfo.set_entity_state(Protocol::ENetworkEntityState::Chase);
			spOwner->ReserveNextState<GameMonsterIdleState>();
			DEFAULT_TRACE_LOG("몬스터 (Attack -> Chase) 전환!");
			return;
		}
	}

	const RxSendBufferPtr& hitDamagePacket = RxServerPacketHandler::I()->MakeHitDamageToEntityPacket(refAttackerInfo, refVictimPlayerInfo);
	RxGameSessionManager::I()->Broadcast(hitDamagePacket);
	DEFAULT_TRACE_LOG("공격 받음! (Victim hp: %d (%p))", remainVictimPlayerHp, m_spVictimPlayer);

	if (bPlayerDeath == true)
	{
		const RxSendBufferPtr& diePlayerPacket = RxServerPacketHandler::I()->MakeDiePlayerPacket(refVictimPlayerInfo);
		RxGameSessionManager::I()->Broadcast(diePlayerPacket);

		GameRoom::I()->RemoveGameEntity(m_spVictimPlayer);
		DEFAULT_TRACE_LOG("개체 사망! (Victim: %p)", m_spVictimPlayer);
	}
}

void GameMonsterAttackState::ToIdleState()
{
	const std::shared_ptr<GameMonster>& spOwner = BringOwner();
	ASSERT_LOG_RETURN(spOwner != nullptr);

	Protocol::NetworkEntityInfo& refGameEntityInfo = spOwner->GetGameEntityInfo();
	refGameEntityInfo.set_entity_state(Protocol::ENetworkEntityState::Idle);
	spOwner->ReserveNextState<GameMonsterIdleState>();
	DEFAULT_TRACE_LOG("몬스터 (Attack -> Idle) 전환!");
}