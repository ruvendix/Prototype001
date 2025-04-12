// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CommunicationActor.h"

Protocol::EEntityLookAtDirection CommunicationActor::JudgeEntityLookAtDirection(const Vector2d& vMoveDir)
{
	Protocol::EEntityLookAtDirection entityLookAtDir = Protocol::EEntityLookAtDirection_MIN;

	if (math::CheckAlikeValue(vMoveDir.x, 1.0f) == true)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Right;
	}
	else if (math::CheckAlikeValue(vMoveDir.x, -1.0f) == true)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Left;
	}

	if (math::CheckAlikeValue(vMoveDir.y, 1.0f) == true)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Down;
	}
	else if (math::CheckAlikeValue(vMoveDir.y, -1.0f) == true)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Up;
	}

	return entityLookAtDir;
}

CommunicationActor::CommunicationActor(const CommunicationActor& src) : Super(src)
{

}

CommunicationActor::~CommunicationActor()
{

}

void CommunicationActor::Startup()
{
	Super::Startup();
	RegisterStateOnBidirectional();
}

bool CommunicationActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void CommunicationActor::Cleanup()
{
	Super::Cleanup();
}

void CommunicationActor::ProcessMoveDirection(const Vector2d& vMoveDir)
{

}

void CommunicationActor::ProcessDefense()
{

}

void CommunicationActor::ProcessAttackAction()
{

}

void CommunicationActor::RegisterStateOnBidirectional()
{
	m_arrActorStateIdMappingEntityStateTable.fill(0);

	RegisterActorStateMappingTable<PawnActorIdleState>(Protocol::EEntityState::Idle);
	RegisterActorStateMappingTable<PawnActorWalkState>(Protocol::EEntityState::Walk);

	RegisterEntityStateMappingTable<PawnActorIdleState>(Protocol::EEntityState::Idle);
	RegisterEntityStateMappingTable<PawnActorWalkState>(Protocol::EEntityState::Walk);
}

void CommunicationActor::SyncFromServer_EntityInfo(const Protocol::EntityInfo& EntityInfo)
{
	m_spEntityInfo = std::make_shared<Protocol::EntityInfo>();
	(*m_spEntityInfo) = EntityInfo;

	Position2d entityCellPos = Position2d(EntityInfo.cell_pos_x(), EntityInfo.cell_pos_y());
	ApplyCellPosition(entityCellPos);

	CellActorMoveComponent* pMoveComponent = GetComponent<CellActorMoveComponent>();
	ASSERT_LOG(pMoveComponent != nullptr);
	pMoveComponent->SetDestinationCellPosition(entityCellPos);

	ApplyActorLookAtDirectionFromServer(EntityInfo.entity_look_at_dir());
	ApplyActorStateFromServer(EntityInfo.entity_state());
}

void CommunicationActor::SyncFromServer_EntityLookAtDirection(const Protocol::EntityInfo& EntityInfo)
{
	ApplyActorLookAtDirectionFromServer(EntityInfo.entity_look_at_dir());
}

void CommunicationActor::SyncFromServer_EntityMove(const Protocol::EntityInfo& EntityInfo)
{
	Position2d entityDestCellPos = Position2d(EntityInfo.cell_pos_x(), EntityInfo.cell_pos_y());
	const Vector2d& vMoveDir = CalculateMoveDirectionByCellPosition(entityDestCellPos);
	ProcessMoveDirection(vMoveDir); // 네트워크 플레이어는 따로 처리해야함
}

void CommunicationActor::SyncFromServer_EntityState(const Protocol::EntityInfo& EntityInfo)
{
	uint32 actorStateId = FindActorStateId(EntityInfo.entity_state());
	ImmediatelyChangeState(actorStateId);
}

void CommunicationActor::SyncFromServer_EntityHitDamage(const CommunicationActorPtr& spAttacker, const Protocol::EntityInfo& victimInfo)
{
	ASSERT_LOG(spAttacker != nullptr);
	ProcessDamaged(spAttacker);

	m_spEntityInfo->set_hp(victimInfo.hp());
}

void CommunicationActor::SyncToServer_EntityInfoIfNeed()
{
	if (m_spEntityInfo == nullptr)
	{
		return;
	}

	SyncToServer_EntityLookAtDirectionIfNeed();
	SyncToServer_EntityMoveIfNeed();
	SyncToServer_EntityAttackIfNeed();
}

EActorLookAtDirection CommunicationActor::ConvertEntityLookAtDirectionToActorLookAtDirection(Protocol::EEntityLookAtDirection entityLookAtDir) const
{
	EActorLookAtDirection actorLookAtDir = EActorLookAtDirection::Count;
	switch (entityLookAtDir)
	{
	case Protocol::EEntityLookAtDirection::Left: (actorLookAtDir = EActorLookAtDirection::Left); break;
	case Protocol::EEntityLookAtDirection::Up: (actorLookAtDir = EActorLookAtDirection::Up); break;
	case Protocol::EEntityLookAtDirection::Right: (actorLookAtDir = EActorLookAtDirection::Right); break;
	case Protocol::EEntityLookAtDirection::Down: (actorLookAtDir = EActorLookAtDirection::Down); break;
	}

	return actorLookAtDir;
}

Protocol::EEntityLookAtDirection CommunicationActor::ConvertActorLookAtDirectionToEntityLookAtDirection(EActorLookAtDirection actorLookAtDir) const
{
	Protocol::EEntityLookAtDirection entityLookAtDir = Protocol::EEntityLookAtDirection::Down;
	switch (actorLookAtDir)
	{
	case EActorLookAtDirection::Left: (entityLookAtDir = Protocol::EEntityLookAtDirection::Left); break;
	case EActorLookAtDirection::Up: (entityLookAtDir = Protocol::EEntityLookAtDirection::Up); break;
	case EActorLookAtDirection::Right: (entityLookAtDir = Protocol::EEntityLookAtDirection::Right); break;
	case EActorLookAtDirection::Down: (entityLookAtDir = Protocol::EEntityLookAtDirection::Down); break;
	}

	return entityLookAtDir;
}

void CommunicationActor::ApplyActorStateFromServer(Protocol::EEntityState entityState)
{
	uint32 CommunicationActorStateId = FindActorStateId(entityState);
	ImmediatelyChangeState(CommunicationActorStateId);
}

void CommunicationActor::ApplyActorLookAtDirectionFromServer(Protocol::EEntityLookAtDirection entityLookAtDir)
{
	SetActorLookAtDirection(ConvertEntityLookAtDirectionToActorLookAtDirection(entityLookAtDir));
	if (ApplyLookAtDirectionSprite() == false)
	{
		// 이것도 실패하면 문제가 있음
		ASSERT_LOG(ApplyLookAtDirectionSpriteOnDefaultState() == true);
	}
}

bool CommunicationActor::CheckClientAndServerIsSameLookAtDirection() const
{
	Protocol::EEntityLookAtDirection entityLookAtDir = m_spEntityInfo->entity_look_at_dir();
	return (entityLookAtDir == ConvertActorLookAtDirectionToEntityLookAtDirection(GetActorLookAtDirection()));
}

bool CommunicationActor::CheckClientAndServerIsSamEEntityState() const
{
	uint32 CommunicationActorStateIdFromServer = FindActorStateId(m_spEntityInfo->entity_state());
	return (GetCurrentPawnActorState()->CompiletimeId() == CommunicationActorStateIdFromServer);
}

Protocol::EEntityState CommunicationActor::FindEntityState(uint32 actorStateId) const
{
	ASSERT_LOG(global::ValidateIndexRange(actorStateId, m_vecEntityStateMappingActorStateIdTable.size()) == true);
	return (m_vecEntityStateMappingActorStateIdTable[actorStateId]);
}

uint32 CommunicationActor::FindActorStateId(Protocol::EEntityState entityState) const
{
	ASEERT_VALIDATE_INDEX(TO_NUM(entityState), m_arrActorStateIdMappingEntityStateTable.size());
	return (m_arrActorStateIdMappingEntityStateTable[TO_NUM(entityState)]);
}

void CommunicationActor::SyncToServer_EntityLookAtDirectionIfNeed()
{
	if (CheckClientAndServerIsSameLookAtDirection() == true)
	{
		return;
	}

	Protocol::EEntityLookAtDirection entityLookAtDir = ConvertActorLookAtDirectionToEntityLookAtDirection(GetActorLookAtDirection());
	m_spEntityInfo->set_entity_look_at_dir(entityLookAtDir);

	const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeModifyEntityLookAtDirectionPacket(m_spEntityInfo);
	NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
}

void CommunicationActor::SyncToServer_EntityMoveIfNeed()
{
	const Position2d& prevDestCellPos =
	{
		static_cast<int32>(m_spEntityInfo->cell_pos_x()),
		static_cast<int32>(m_spEntityInfo->cell_pos_y())
	};

	CellActorMoveComponent* pMoveComponent = GetComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);

	const Position2d& currentDestCellPos = pMoveComponent->GetDestinationCellPosition();
	if (prevDestCellPos != currentDestCellPos)
	{
		// 서버로 요청할 이동 관련 정보
		m_spEntityInfo->set_cell_pos_x(currentDestCellPos.x);
		m_spEntityInfo->set_cell_pos_y(currentDestCellPos.y);

		const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeMoveEntityPacket(m_spEntityInfo);
		NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
	}
}

void CommunicationActor::SyncToServer_EntityAttackIfNeed()
{
	if (CheckClientAndServerIsSamEEntityState() == true)
	{
		return;
	}

	Protocol::EEntityState entityState = FindEntityState(GetCurrentPawnActorState()->CompiletimeId());
	m_spEntityInfo->set_entity_state(entityState);

	const RxSendBufferPtr& spSendSyncGamePlayerPacket = ClientPacketHandler::I()->MakeModifyEntityStatePacket(m_spEntityInfo);
	NetworkManager::I()->SendPacket(spSendSyncGamePlayerPacket);
}