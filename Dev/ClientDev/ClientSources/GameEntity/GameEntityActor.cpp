// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameEntityActor.h"

GameEntityActor::~GameEntityActor()
{

}

void GameEntityActor::Startup()
{
	Super::Startup();
}

bool GameEntityActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	SyncToServer_GameEntityInfoIfNeed();
	return true;
}

void GameEntityActor::Cleanup()
{
	Super::Cleanup();
}

void GameEntityActor::ProcessMoveDirection(const Vector2d& vMoveDir)
{

}

void GameEntityActor::SyncFromServer_GameEntityInfo(const Protocol::GameEntityInfo& gameEntityInfo)
{
	m_spGameEntityInfo = std::make_shared<Protocol::GameEntityInfo>();
	(*m_spGameEntityInfo) = gameEntityInfo;

	Position2d gameEntityCellPos = Position2d(gameEntityInfo.cell_pos_x(), gameEntityInfo.cell_pos_y());
	ApplyCellPosition(gameEntityCellPos);

	CellActorMoveComponent* pMoveComponent = FindComponent<CellActorMoveComponent>();
	ASSERT_LOG(pMoveComponent != nullptr);
	pMoveComponent->SetDestinationCellPosition(gameEntityCellPos);
}

void GameEntityActor::SyncFromServer_GameEntityMove(const Protocol::GameEntityInfo& gameEntityInfo)
{
	Position2d gameEntityDestCellPos = Position2d(gameEntityInfo.cell_pos_x(), gameEntityInfo.cell_pos_y());
	const Vector2d& vMoveDir = CalculateMoveDirectionByCellPosition(gameEntityDestCellPos);
	ProcessMoveDirection(vMoveDir);
}

void GameEntityActor::SyncToServer_GameEntityInfoIfNeed()
{
	if (m_spGameEntityInfo == nullptr)
	{
		return;
	}

	SyncToServer_GameEntityMoveIfNeed();
}

void GameEntityActor::SyncToServer_GameEntityMoveIfNeed()
{
	const Position2d& prevDestCellPos =
	{
		static_cast<int32>(m_spGameEntityInfo->cell_pos_x()),
		static_cast<int32>(m_spGameEntityInfo->cell_pos_y())
	};

	CellActorMoveComponent* pMoveComponent = FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);

	const Position2d& currentDestCellPos = pMoveComponent->GetDestinationCellPosition();
	if (prevDestCellPos != currentDestCellPos)
	{
		// 이동시킬 좌표
		m_spGameEntityInfo->set_cell_pos_x(currentDestCellPos.x);
		m_spGameEntityInfo->set_cell_pos_y(currentDestCellPos.y);

		const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeSyncGamePlayerMovePacket(m_spGameEntityInfo);
		NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
	}
}