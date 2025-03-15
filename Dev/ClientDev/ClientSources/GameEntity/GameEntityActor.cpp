// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameEntityActor.h"

GameEntityActor::~GameEntityActor()
{

}

void GameEntityActor::Startup()
{
	Super::Startup();

	RegisterActorState<PawnActorIdleState>(Protocol::EGameEntityState::Idle);
	RegisterActorState<PawnActorWalkState>(Protocol::EGameEntityState::Walk);
	RegisterActorState<PawnActorAttackState>(Protocol::EGameEntityState::Attack);
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

	ApplyActorLookAtDirectionFromServer(gameEntityInfo.entitye_look_at_dir());
	ApplyActorStateFromServer(gameEntityInfo.entity_state());
}

void GameEntityActor::SyncFromServer_GameEntityLookAtDirection(const Protocol::GameEntityInfo& gameEntityInfo)
{
	ApplyActorLookAtDirectionFromServer(gameEntityInfo.entitye_look_at_dir());
}

void GameEntityActor::SyncFromServer_GameEntityMove(const Protocol::GameEntityInfo& gameEntityInfo)
{
	Position2d gameEntityDestCellPos = Position2d(gameEntityInfo.cell_pos_x(), gameEntityInfo.cell_pos_y());
	const Vector2d& vMoveDir = CalculateMoveDirectionByCellPosition(gameEntityDestCellPos);
	ProcessMoveDirection(vMoveDir); // 네트워크 플레이어는 따로 처리해야함
}

void GameEntityActor::SyncToServer_GameEntityInfoIfNeed()
{
	if (m_spGameEntityInfo == nullptr)
	{
		return;
	}

	SyncToServer_GameEntityLookAtDirectionIfNeed();
	SyncToServer_GameEntityMoveIfNeed();
}

EActorLookAtDirection GameEntityActor::ConvertGameEntityLookAtDirectionToActorLookAtDirection(Protocol::EGameEntityLookAtDir gameEntityLookAtDir) const
{
	EActorLookAtDirection actorLookAtDir = EActorLookAtDirection::Count;
	switch (gameEntityLookAtDir)
	{
	case Protocol::EGameEntityLookAtDir::Left: (actorLookAtDir = EActorLookAtDirection::Left); break;
	case Protocol::EGameEntityLookAtDir::Up: (actorLookAtDir = EActorLookAtDirection::Up); break;
	case Protocol::EGameEntityLookAtDir::Right: (actorLookAtDir = EActorLookAtDirection::Right); break;
	case Protocol::EGameEntityLookAtDir::Down: (actorLookAtDir = EActorLookAtDirection::Down); break;
	}

	return actorLookAtDir;
}

Protocol::EGameEntityLookAtDir GameEntityActor::ConvertActorLookAtDirectionToGameEntityLookAtDirection(EActorLookAtDirection actorLookAtDir) const
{
	Protocol::EGameEntityLookAtDir gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Count;
	switch (actorLookAtDir)
	{
	case EActorLookAtDirection::Left: (gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Left); break;
	case EActorLookAtDirection::Up: (gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Up); break;
	case EActorLookAtDirection::Right: (gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Right); break;
	case EActorLookAtDirection::Down: (gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Down); break;
	}

	return gameEntityLookAtDir;
}

void GameEntityActor::ApplyActorStateFromServer(Protocol::EGameEntityState gameEntityState)
{
	auto foundIter = m_mapActorState.find(TO_NUM(gameEntityState));
	if (foundIter == m_mapActorState.cend())
	{
		ERROR_LOG(LogDefault, "등록되지 않은 액터의 상태 Id! (%d)", TO_NUM(gameEntityState));
	}

	ImmediatelyChangeStateByExternal(foundIter->second);
}

void GameEntityActor::ApplyActorLookAtDirectionFromServer(Protocol::EGameEntityLookAtDir gameEntityLookAtDir)
{
	SetActorLookAtDirection(ConvertGameEntityLookAtDirectionToActorLookAtDirection(gameEntityLookAtDir));
	ApplyLookAtDirectionToCurrentDynamicSprite();
}

bool GameEntityActor::CheckClientAndServerIsSameLookAtDirection() const
{
	Protocol::EGameEntityLookAtDir gameEntityLookAtDir = m_spGameEntityInfo->entitye_look_at_dir();
	return (gameEntityLookAtDir == ConvertActorLookAtDirectionToGameEntityLookAtDirection(GetActorLookAtDirection()));
}

void GameEntityActor::SyncToServer_GameEntityLookAtDirectionIfNeed()
{
	if (CheckClientAndServerIsSameLookAtDirection() == true)
	{
		return;
	}

	Protocol::EGameEntityLookAtDir gameEntityLookAtDir = ConvertActorLookAtDirectionToGameEntityLookAtDirection(GetActorLookAtDirection());
	m_spGameEntityInfo->set_entitye_look_at_dir(gameEntityLookAtDir);

	const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeSyncGameEntityLookAtDirectionPacket(m_spGameEntityInfo);
	NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
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
		// 서버로 요청할 이동 관련 정보
		m_spGameEntityInfo->set_cell_pos_x(currentDestCellPos.x);
		m_spGameEntityInfo->set_cell_pos_y(currentDestCellPos.y);
		m_spGameEntityInfo->set_entity_state(Protocol::EGameEntityState::Walk);

		const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeSyncGamePlayerMovePacket(m_spGameEntityInfo);
		NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
	}
}