// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class NetworkEntityActor : public PawnActor
{
	using Super = PawnActor;

public:
	using Super::Super;
	NetworkEntityActor(const NetworkEntityActor& src);
	virtual ~NetworkEntityActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessMoveDirection(const Vector2d& vMoveDir);
	virtual void ProcessDefense();
	virtual void ProcessAttackAction();

	virtual void RegisterStateOnBidirectional();

public:
	template <typename TActorState>
	void RegisterActorStateMappingTable(Protocol::ENetworkEntityState networkEntityState)
	{
		ASEERT_VALIDATE_INDEX(TO_NUM(networkEntityState), m_arrActorStateIdMappingNetworkEntityStateTable.size());
		m_arrActorStateIdMappingNetworkEntityStateTable[TO_NUM(networkEntityState)] = TActorState::s_id;
	}

	template <typename TActorState>
	void RegisterNetworkEntityStateMappingTable(Protocol::ENetworkEntityState networkEntityState)
	{
		if (m_vecNetworkEntityStateMappingActorStateIdTable.empty() == true)
		{
			m_vecNetworkEntityStateMappingActorStateIdTable.resize(GET_COMPILEITME_ID_COUNT(PawnActorStateIdCounter));
		}

		ASEERT_VALIDATE_INDEX(TActorState::s_id, m_vecNetworkEntityStateMappingActorStateIdTable.size());
		m_vecNetworkEntityStateMappingActorStateIdTable[TActorState::s_id] = networkEntityState;
	}

public:
	void SyncFromServer_NetworkEntityInfo(const Protocol::NetworkEntityInfo& networkEntityInfo);
	void SyncFromServer_NetworkEntityLookAtDirection(const Protocol::NetworkEntityInfo& networkEntityInfo);
	void SyncFromServer_NetworkEntityMove(const Protocol::NetworkEntityInfo& networkEntityInfo);
	void SyncFromServer_NetworkEntityState(const Protocol::NetworkEntityInfo& networkEntityInfo);
	void SyncFromServer_NetworkEntityHitDamage(const NetworkEntityActorPtr& spAttacker, const Protocol::NetworkEntityInfo& victimInfo);

	void SyncToServer_NetworkEntityInfoIfNeed();

	EActorLookAtDirection ConvertNetworkEntityLookAtDirectionToActorLookAtDirection(Protocol::ENetworkEntityLookAtDirection networkEntityLookAtDir) const;
	Protocol::ENetworkEntityLookAtDirection ConvertActorLookAtDirectionToNetworkEntityLookAtDirection(EActorLookAtDirection actorLookAtDir) const;

	void ApplyActorStateFromServer(Protocol::ENetworkEntityState networkEntityState);
	void ApplyActorLookAtDirectionFromServer(Protocol::ENetworkEntityLookAtDirection networkEntityLookAtDir);

	bool CheckClientAndServerIsSameLookAtDirection() const;
	bool CheckClientAndServerIsSameNetworkEntityState() const;

	Protocol::ENetworkEntityState FindNetworkEntityState(uint32 actorStateId) const;
	uint32 FindActorStateId(Protocol::ENetworkEntityState networkEntityState) const;

	uint64 GetNetworkEntityId() const { return (m_spNetworkEntityInfo->entity_id()); }
	Protocol::ENetworkEntityState GetNetworkEntityState() const { return (m_spNetworkEntityInfo->entity_state()); }

	bool IsCreatedNetworkEntityInfo() const { return (m_spNetworkEntityInfo != nullptr); }

private:
	void SyncToServer_NetworkEntityLookAtDirectionIfNeed();
	void SyncToServer_NetworkEntityMoveIfNeed();
	void SyncToServer_NetworkEntityAttackIfNeed();

private:
	// 네트워크 개체 상태에 해당되는 액터의 상태
	std::array<uint32, Protocol::ENetworkEntityState_ARRAYSIZE> m_arrActorStateIdMappingNetworkEntityStateTable;
	std::vector<Protocol::ENetworkEntityState> m_vecNetworkEntityStateMappingActorStateIdTable;

	std::shared_ptr<Protocol::NetworkEntityInfo> m_spNetworkEntityInfo;
};