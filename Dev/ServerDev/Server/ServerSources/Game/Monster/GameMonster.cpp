// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonster.h"

GameMonster::GameMonster()
{

}

GameMonster::~GameMonster()
{

}

void GameMonster::Startup()
{
	m_timerOnIdle = TimerManager::I()->CreateTimer(0.5f, true, this, &GameMonster::OnTimerIdle);
	m_timerOnWalk = TimerManager::I()->CreateTimer(1.0f, true, this, &GameMonster::OnTimerWalk);
}

bool GameMonster::Update(float deltaSeconds)
{
	// Idle ���¸� ���ƴٴ�
	switch (GetGameEntityInfo().entity_state())
	{
	case Protocol::EGameEntityState::Idle:
		UpdateIdle(deltaSeconds);
		break;

	case Protocol::EGameEntityState::Walk:
		UpdateWalk(deltaSeconds);
		break;
	}

	return true;
}

void GameMonster::Cleanup()
{

}

void GameMonster::UpdateIdle(float deltaSeconds)
{
	m_timerOnIdle.Update(deltaSeconds);
}

void GameMonster::UpdateWalk(float deltaSeconds)
{
	m_timerOnWalk.Update(deltaSeconds);
}

void GameMonster::ApplyGameMonsterInfo(const Protocol::GameMonsterInfo& srcGameMonsterInfo)
{
	m_monsterId = srcGameMonsterInfo.monster_id();
	SetGameEntityInfo(srcGameMonsterInfo.entity_info());
}

void GameMonster::CopyGameMonsterInfo(Protocol::GameMonsterInfo* pDestGameMonsterInfo)
{
	pDestGameMonsterInfo->set_monster_id(m_monsterId);
	*(pDestGameMonsterInfo->mutable_entity_info()) = GetGameEntityInfo();
}

void GameMonster::OnTimerIdle()
{
	Protocol::GameEntityInfo gameEntityInfo = GetGameEntityInfo();

	// �̵� ������ ���� ��ã������ ���� ��ȸ��
	int32 randMoveDirIdx = (std::rand() % TO_NUM(Protocol::EGameEntityLookAtDir::Count));
	const Position2d& moveDir = PawnActor::g_lookAtForwardCellPosTable[randMoveDirIdx];

	Position2d destCellPos =
	{ 
		static_cast<int32>(gameEntityInfo.cell_pos_x()) + moveDir.x,
		static_cast<int32>(gameEntityInfo.cell_pos_y()) + moveDir.y
	};

	if (GameRoom::I()->CheckCanMoveToCellPosition(destCellPos, nullptr) == false)
	{
		return;
	}

	gameEntityInfo.set_cell_pos_x(destCellPos.x);
	gameEntityInfo.set_cell_pos_y(destCellPos.y);
	gameEntityInfo.set_entity_state(Protocol::EGameEntityState::Walk);
	gameEntityInfo.set_entitye_look_at_dir(static_cast<Protocol::EGameEntityLookAtDir>(randMoveDirIdx));
	SetGameEntityInfo(gameEntityInfo);

	const RxSendBufferPtr& syncGameEntityMovePacket = RxServerPacketHandler::I()->MakeSyncGameEntityMovePacket(gameEntityInfo);
	RxGameSessionManager::I()->Broadcast(syncGameEntityMovePacket);

	m_timerOnIdle.SetOff();
	m_timerOnWalk.SetOn();
	m_timerOnWalk.Reset();

	DEFAULT_TRACE_LOG("�� Walk ��ȯ!");
}

void GameMonster::OnTimerWalk()
{
	Protocol::GameEntityInfo gameEntityInfo = GetGameEntityInfo();
	gameEntityInfo.set_entity_state(Protocol::EGameEntityState::Idle);
	SetGameEntityInfo(gameEntityInfo);

	const RxSendBufferPtr& syncGameEntityStatePacket = RxServerPacketHandler::I()->MakeSyncGameEntityStatePacket(gameEntityInfo);
	RxGameSessionManager::I()->Broadcast(syncGameEntityStatePacket);
	
	m_timerOnWalk.SetOff();
	m_timerOnIdle.SetOn();
	m_timerOnIdle.Reset();

	DEFAULT_TRACE_LOG("�� Idle ��ȯ!");
}