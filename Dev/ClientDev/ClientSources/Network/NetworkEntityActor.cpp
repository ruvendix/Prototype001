// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "NetworkEntityActor.h"

NetworkEntityActor::~NetworkEntityActor()
{

}

void NetworkEntityActor::Startup()
{
	Super::Startup();
	RegisterStateOnBidirectional();
}

bool NetworkEntityActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void NetworkEntityActor::Cleanup()
{
	Super::Cleanup();
}

void NetworkEntityActor::ProcessMoveDirection(const Vector2d& vMoveDir)
{

}

void NetworkEntityActor::ProcessDefense()
{

}

void NetworkEntityActor::ProcessAttack()
{

}

void NetworkEntityActor::RegisterStateOnBidirectional()
{
	RegisterActorState<PawnActorIdleState>(Protocol::ENetworkEntityState::Idle);
	RegisterActorState<PawnActorWalkState>(Protocol::ENetworkEntityState::Walk);
	RegisterActorState<PawnActorAttackState>(Protocol::ENetworkEntityState::Attack);

	RegisterNetworkEntityState<PawnActorIdleState>(Protocol::ENetworkEntityState::Idle);
	RegisterNetworkEntityState<PawnActorWalkState>(Protocol::ENetworkEntityState::Walk);
	RegisterNetworkEntityState<PawnActorAttackState>(Protocol::ENetworkEntityState::Attack);
}

void NetworkEntityActor::SyncFromServer_NetworkEntityInfo(const Protocol::NetworkEntityInfo& networkEntityInfo)
{
	m_spNetworkEntityInfo = std::make_shared<Protocol::NetworkEntityInfo>();
	(*m_spNetworkEntityInfo) = networkEntityInfo;

	Position2d networkEntityCellPos = Position2d(networkEntityInfo.cell_pos_x(), networkEntityInfo.cell_pos_y());
	ApplyCellPosition(networkEntityCellPos);

	CellActorMoveComponent* pMoveComponent = GetComponent<CellActorMoveComponent>();
	ASSERT_LOG(pMoveComponent != nullptr);
	pMoveComponent->SetDestinationCellPosition(networkEntityCellPos);

	ApplyActorLookAtDirectionFromServer(networkEntityInfo.entitye_look_at_dir());
	ApplyActorStateFromServer(networkEntityInfo.entity_state());
}

void NetworkEntityActor::SyncFromServer_NetworkEntityLookAtDirection(const Protocol::NetworkEntityInfo& networkEntityInfo)
{
	ApplyActorLookAtDirectionFromServer(networkEntityInfo.entitye_look_at_dir());
}

void NetworkEntityActor::SyncFromServer_NetworkEntityMove(const Protocol::NetworkEntityInfo& networkEntityInfo)
{
	Position2d networkEntityDestCellPos = Position2d(networkEntityInfo.cell_pos_x(), networkEntityInfo.cell_pos_y());
	const Vector2d& vMoveDir = CalculateMoveDirectionByCellPosition(networkEntityDestCellPos);
	ProcessMoveDirection(vMoveDir); // 네트워크 플레이어는 따로 처리해야함
}

void NetworkEntityActor::SyncFromServer_NetworkEntityState(const Protocol::NetworkEntityInfo& networkEntityInfo)
{
	const PawnActorStatePtr& spActorState = FindActorState(networkEntityInfo.entity_state());
	ImmediatelyChangeStateByExternal(spActorState);
}

void NetworkEntityActor::SyncFromServer_NetworkEntityHitDamage(const NetworkEntityActorPtr& spAttacker, const Protocol::NetworkEntityInfo& victimInfo)
{
	ASSERT_LOG(spAttacker != nullptr);
	ProcessDamaged(spAttacker);

	m_spNetworkEntityInfo->set_hp(victimInfo.hp());
}

void NetworkEntityActor::SyncToServer_NetworkEntityInfoIfNeed()
{
	if (m_spNetworkEntityInfo == nullptr)
	{
		return;
	}

	SyncToServer_NetworkEntityLookAtDirectionIfNeed();
	SyncToServer_NetworkEntityMoveIfNeed();
	SyncToServer_NetworkEntityAttackIfNeed();
}

EActorLookAtDirection NetworkEntityActor::ConvertNetworkEntityLookAtDirectionToActorLookAtDirection(Protocol::ENetworkEntityLookAtDirection networkEntityLookAtDir) const
{
	EActorLookAtDirection actorLookAtDir = EActorLookAtDirection::Count;
	switch (networkEntityLookAtDir)
	{
	case Protocol::ENetworkEntityLookAtDirection::Left: (actorLookAtDir = EActorLookAtDirection::Left); break;
	case Protocol::ENetworkEntityLookAtDirection::Up: (actorLookAtDir = EActorLookAtDirection::Up); break;
	case Protocol::ENetworkEntityLookAtDirection::Right: (actorLookAtDir = EActorLookAtDirection::Right); break;
	case Protocol::ENetworkEntityLookAtDirection::Down: (actorLookAtDir = EActorLookAtDirection::Down); break;
	}

	return actorLookAtDir;
}

Protocol::ENetworkEntityLookAtDirection NetworkEntityActor::ConvertActorLookAtDirectionToNetworkEntityLookAtDirection(EActorLookAtDirection actorLookAtDir) const
{
	Protocol::ENetworkEntityLookAtDirection networkEntityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Count;
	switch (actorLookAtDir)
	{
	case EActorLookAtDirection::Left: (networkEntityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Left); break;
	case EActorLookAtDirection::Up: (networkEntityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Up); break;
	case EActorLookAtDirection::Right: (networkEntityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Right); break;
	case EActorLookAtDirection::Down: (networkEntityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Down); break;
	}

	return networkEntityLookAtDir;
}

void NetworkEntityActor::ApplyActorStateFromServer(Protocol::ENetworkEntityState networkEntityState)
{
	ImmediatelyChangeStateByExternal(FindActorState(networkEntityState));
}

void NetworkEntityActor::ApplyActorLookAtDirectionFromServer(Protocol::ENetworkEntityLookAtDirection networkEntityLookAtDir)
{
	SetActorLookAtDirection(ConvertNetworkEntityLookAtDirectionToActorLookAtDirection(networkEntityLookAtDir));
	if (ApplyLookAtDirectionSprite() == false)
	{
		// 이것도 실패하면 문제가 있음
		ASSERT_LOG(ApplyLookAtDirectionSpriteOnDefaultState() == true);
	}
}

bool NetworkEntityActor::CheckClientAndServerIsSameLookAtDirection() const
{
	Protocol::ENetworkEntityLookAtDirection networkEntityLookAtDir = m_spNetworkEntityInfo->entitye_look_at_dir();
	return (networkEntityLookAtDir == ConvertActorLookAtDirectionToNetworkEntityLookAtDirection(GetActorLookAtDirection()));
}

bool NetworkEntityActor::CheckClientAndServerIsSameNetworkEntityState() const
{
	const PawnActorStatePtr& spActorStateFromServer = FindActorState(m_spNetworkEntityInfo->entity_state());
	return (spActorStateFromServer->CompiletimeId() == GetCurrentPawnActorState()->CompiletimeId());
}

Protocol::ENetworkEntityState NetworkEntityActor::FindNetworkEntityState(uint32 actorStateId) const
{
	auto foundIter = m_mapNetworkEntityState.find(actorStateId);
	if (foundIter == m_mapNetworkEntityState.cend())
	{
		ERROR_LOG(LogDefault, "등록되지 않은 게임 엔티티의 상태 Id! (%d)", actorStateId);
	}

	return static_cast<Protocol::ENetworkEntityState>(foundIter->second);
}

PawnActorStatePtr NetworkEntityActor::FindActorState(Protocol::ENetworkEntityState networkEntityState) const
{
	auto foundIter = m_mapActorState.find(TO_NUM(networkEntityState));
	if (foundIter == m_mapActorState.cend())
	{
		ERROR_LOG(LogDefault, "등록되지 않은 액터의 상태 Id! (%d)", TO_NUM(networkEntityState));
	}

	return (foundIter->second);
}

void NetworkEntityActor::SyncToServer_NetworkEntityLookAtDirectionIfNeed()
{
	if (CheckClientAndServerIsSameLookAtDirection() == true)
	{
		return;
	}

	Protocol::ENetworkEntityLookAtDirection networkEntityLookAtDir = ConvertActorLookAtDirectionToNetworkEntityLookAtDirection(GetActorLookAtDirection());
	m_spNetworkEntityInfo->set_entitye_look_at_dir(networkEntityLookAtDir);

	const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeModifyEntityLookAtDirectionPacket(m_spNetworkEntityInfo);
	NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
}

void NetworkEntityActor::SyncToServer_NetworkEntityMoveIfNeed()
{
	const Position2d& prevDestCellPos =
	{
		static_cast<int32>(m_spNetworkEntityInfo->cell_pos_x()),
		static_cast<int32>(m_spNetworkEntityInfo->cell_pos_y())
	};

	CellActorMoveComponent* pMoveComponent = GetComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);

	const Position2d& currentDestCellPos = pMoveComponent->GetDestinationCellPosition();
	if (prevDestCellPos != currentDestCellPos)
	{
		// 서버로 요청할 이동 관련 정보
		m_spNetworkEntityInfo->set_cell_pos_x(currentDestCellPos.x);
		m_spNetworkEntityInfo->set_cell_pos_y(currentDestCellPos.y);

		const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeMoveEntityPacket(m_spNetworkEntityInfo);
		NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
	}
}

void NetworkEntityActor::SyncToServer_NetworkEntityAttackIfNeed()
{
	if (CheckClientAndServerIsSameNetworkEntityState() == true)
	{
		return;
	}

	Protocol::ENetworkEntityState networkEntityState = FindNetworkEntityState(GetCurrentPawnActorState()->CompiletimeId());
	m_spNetworkEntityInfo->set_entity_state(networkEntityState);

	const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeModifyEntityStatePacket(m_spNetworkEntityInfo);
	NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
}