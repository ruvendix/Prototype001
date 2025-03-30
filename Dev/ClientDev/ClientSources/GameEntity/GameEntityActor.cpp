// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameEntityActor.h"

GameEntityActor::~GameEntityActor()
{

}

void GameEntityActor::Startup()
{
	Super::Startup();
	RegisterStateOnBidirectional();
}

bool GameEntityActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void GameEntityActor::Cleanup()
{
	Super::Cleanup();
}

void GameEntityActor::ProcessMoveDirection(const Vector2d& vMoveDir)
{

}

void GameEntityActor::ProcessDefense()
{

}

void GameEntityActor::ProcessAttack()
{

}

void GameEntityActor::RegisterStateOnBidirectional()
{
	RegisterActorState<PawnActorIdleState>(Protocol::EGameEntityState::Idle);
	RegisterActorState<PawnActorWalkState>(Protocol::EGameEntityState::Walk);
	RegisterActorState<PawnActorAttackState>(Protocol::EGameEntityState::Attack);

	RegisterGameEntityState<PawnActorIdleState>(Protocol::EGameEntityState::Idle);
	RegisterGameEntityState<PawnActorWalkState>(Protocol::EGameEntityState::Walk);
	RegisterGameEntityState<PawnActorAttackState>(Protocol::EGameEntityState::Attack);
}

void GameEntityActor::SyncFromServer_GameEntityInfo(const Protocol::GameEntityInfo& gameEntityInfo)
{
	m_spGameEntityInfo = std::make_shared<Protocol::GameEntityInfo>();
	(*m_spGameEntityInfo) = gameEntityInfo;

	Position2d gameEntityCellPos = Position2d(gameEntityInfo.cell_pos_x(), gameEntityInfo.cell_pos_y());
	ApplyCellPosition(gameEntityCellPos);

	CellActorMoveComponent* pMoveComponent = GetComponent<CellActorMoveComponent>();
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

void GameEntityActor::SyncFromServer_GameEntityState(const Protocol::GameEntityInfo& gameEntityInfo)
{
	const PawnActorStatePtr& spActorState = FindActorState(gameEntityInfo.entity_state());
	ImmediatelyChangeStateByExternal(spActorState);
}

void GameEntityActor::SyncFromServer_AttackToGameEntity(const GameEntityActorPtr& spAttacker, const Protocol::GameEntityInfo& victimInfo)
{
	ASSERT_LOG(spAttacker != nullptr);
	ProcessDamaged(spAttacker);

	m_spGameEntityInfo->set_hp(victimInfo.hp());
}

void GameEntityActor::SyncToServer_GameEntityInfoIfNeed()
{
	if (m_spGameEntityInfo == nullptr)
	{
		return;
	}

	SyncToServer_GameEntityLookAtDirectionIfNeed();
	SyncToServer_GameEntityMoveIfNeed();
	SyncToServer_GameEntityAttackIfNeed();
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
	ImmediatelyChangeStateByExternal(FindActorState(gameEntityState));
}

void GameEntityActor::ApplyActorLookAtDirectionFromServer(Protocol::EGameEntityLookAtDir gameEntityLookAtDir)
{
	SetActorLookAtDirection(ConvertGameEntityLookAtDirectionToActorLookAtDirection(gameEntityLookAtDir));
	if (ApplyLookAtDirectionSprite() == false)
	{
		// 이것도 실패하면 문제가 있음
		ASSERT_LOG(ApplyLookAtDirectionSpriteOnDefaultState() == true);
	}
}

bool GameEntityActor::CheckClientAndServerIsSameLookAtDirection() const
{
	Protocol::EGameEntityLookAtDir gameEntityLookAtDir = m_spGameEntityInfo->entitye_look_at_dir();
	return (gameEntityLookAtDir == ConvertActorLookAtDirectionToGameEntityLookAtDirection(GetActorLookAtDirection()));
}

bool GameEntityActor::CheckClientAndServerIsSameGameEntityState() const
{
	const PawnActorStatePtr& spActorStateFromServer = FindActorState(m_spGameEntityInfo->entity_state());
	return (spActorStateFromServer->CompiletimeId() == GetCurrentPawnActorState()->CompiletimeId());
}

Protocol::EGameEntityState GameEntityActor::FindGameEntityState(uint32 actorStateId) const
{
	auto foundIter = m_mapGameEntityState.find(actorStateId);
	if (foundIter == m_mapGameEntityState.cend())
	{
		ERROR_LOG(LogDefault, "등록되지 않은 게임 엔티티의 상태 Id! (%d)", actorStateId);
	}

	return static_cast<Protocol::EGameEntityState>(foundIter->second);
}

PawnActorStatePtr GameEntityActor::FindActorState(Protocol::EGameEntityState gameEntityState) const
{
	auto foundIter = m_mapActorState.find(TO_NUM(gameEntityState));
	if (foundIter == m_mapActorState.cend())
	{
		ERROR_LOG(LogDefault, "등록되지 않은 액터의 상태 Id! (%d)", TO_NUM(gameEntityState));
	}

	return (foundIter->second);
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

	CellActorMoveComponent* pMoveComponent = GetComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);

	const Position2d& currentDestCellPos = pMoveComponent->GetDestinationCellPosition();
	if (prevDestCellPos != currentDestCellPos)
	{
		// 서버로 요청할 이동 관련 정보
		m_spGameEntityInfo->set_cell_pos_x(currentDestCellPos.x);
		m_spGameEntityInfo->set_cell_pos_y(currentDestCellPos.y);

		const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeSyncGamePlayerMovePacket(m_spGameEntityInfo);
		NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
	}
}

void GameEntityActor::SyncToServer_GameEntityAttackIfNeed()
{
	if (CheckClientAndServerIsSameGameEntityState() == true)
	{
		return;
	}

	Protocol::EGameEntityState gameEntityState = FindGameEntityState(GetCurrentPawnActorState()->CompiletimeId());
	m_spGameEntityInfo->set_entity_state(gameEntityState);

	const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeSyncGameEntityStatePacket(m_spGameEntityInfo);
	NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
}