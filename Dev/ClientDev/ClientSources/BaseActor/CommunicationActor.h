// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class CommunicationActor : public PawnActor
{
	using Super = PawnActor;

public:
	static Protocol::EEntityLookAtDirection JudgeEntityLookAtDirection(const Vector2d& vMoveDir);

public:
	using Super::Super;
	CommunicationActor(const CommunicationActor& src);
	virtual ~CommunicationActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessMoveDirection(const Vector2d& vMoveDir);
	virtual void ProcessDefense();
	virtual void ProcessAttackAction();

	virtual void RegisterStateOnBidirectional();

public:
	template <typename TActorState>
	void RegisterActorStateMappingTable(Protocol::EEntityState entityState)
	{
		ASEERT_VALIDATE_INDEX(TO_NUM(entityState), m_arrActorStateIdMappingEntityStateTable.size());
		m_arrActorStateIdMappingEntityStateTable[TO_NUM(entityState)] = TActorState::s_id;
	}

	template <typename TActorState>
	void RegisterEntityStateMappingTable(Protocol::EEntityState entityState)
	{
		if (m_vecEntityStateMappingActorStateIdTable.empty() == true)
		{
			m_vecEntityStateMappingActorStateIdTable.resize(GET_COMPILEITME_ID_COUNT(PawnActorStateIdCounter));
		}

		ASEERT_VALIDATE_INDEX(TActorState::s_id, m_vecEntityStateMappingActorStateIdTable.size());
		m_vecEntityStateMappingActorStateIdTable[TActorState::s_id] = entityState;
	}

public:
	void SyncFromServer_EntityInfo(const Protocol::EntityInfo& EntityInfo);
	void SyncFromServer_EntityLookAtDirection(const Protocol::EntityInfo& EntityInfo);
	void SyncFromServer_EntityMove(const Protocol::EntityInfo& EntityInfo);
	void SyncFromServer_EntityState(const Protocol::EntityInfo& EntityInfo);
	void SyncFromServer_EntityHitDamage(const CommunicationActorPtr& spAttacker, const Protocol::EntityInfo& victimInfo);

	void SyncToServer_EntityInfoIfNeed();

	EActorLookAtDirection ConvertEntityLookAtDirectionToActorLookAtDirection(Protocol::EEntityLookAtDirection entityLookAtDir) const;
	Protocol::EEntityLookAtDirection ConvertActorLookAtDirectionToEntityLookAtDirection(EActorLookAtDirection actorLookAtDir) const;

	void ApplyActorStateFromServer(Protocol::EEntityState entityState);
	void ApplyActorLookAtDirectionFromServer(Protocol::EEntityLookAtDirection entityLookAtDir);

	bool CheckClientAndServerIsSameLookAtDirection() const;
	bool CheckClientAndServerIsSamEEntityState() const;

	Protocol::EEntityState FindEntityState(uint32 actorStateId) const;
	uint32 FindActorStateId(Protocol::EEntityState entityState) const;

	uint64 GetEntityId() const { return (m_spEntityInfo->entity_id()); }
	Protocol::EEntityState GetEntityState() const { return (m_spEntityInfo->entity_state()); }
	const ProtocolEntityInfoPtr& GetEntityInfo() const { return m_spEntityInfo; }

	bool IsCreatedEntityInfo() const { return (m_spEntityInfo != nullptr); }

private:
	void SyncToServer_EntityLookAtDirectionIfNeed();
	void SyncToServer_EntityMoveIfNeed();
	void SyncToServer_EntityAttackIfNeed();

private:
	// 서버와 통신하는 개체 상태에 해당되는 액터의 상태
	std::array<uint32, Protocol::EEntityState_ARRAYSIZE> m_arrActorStateIdMappingEntityStateTable;
	std::vector<Protocol::EEntityState> m_vecEntityStateMappingActorStateIdTable;

	ProtocolEntityInfoPtr m_spEntityInfo;
};