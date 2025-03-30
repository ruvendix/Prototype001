// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class NetworkEntityActor : public PawnActor
{
	using Super = PawnActor;

public:
	using Super::Super;
	virtual ~NetworkEntityActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessMoveDirection(const Vector2d& vMoveDir);
	virtual void ProcessDefense();
	virtual void ProcessAttack();

	virtual void RegisterStateOnBidirectional();

public:
	template <typename TActorState>
	void RegisterActorState(Protocol::ENetworkEntityState networkEntityState)
	{
		auto foundIter = m_mapActorState.find(TO_NUM(networkEntityState));
		if (foundIter != m_mapActorState.cend())
		{
			return;
		}

		auto insertedIter = m_mapActorState.insert(std::make_pair(TO_NUM(networkEntityState), std::make_shared<TActorState>(this)));
		ASSERT_LOG(insertedIter.second == true);
	}

	template <typename TActorState>
	void RegisterNetworkEntityState(Protocol::ENetworkEntityState networkEntityState)
	{
		auto foundIter = m_mapNetworkEntityState.find(TActorState::s_id);
		if (foundIter != m_mapNetworkEntityState.cend())
		{
			return;
		}

		auto insertedIter = m_mapNetworkEntityState.insert(std::make_pair(TActorState::s_id, TO_NUM(networkEntityState)));
		ASSERT_LOG(insertedIter.second == true);
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
	PawnActorStatePtr FindActorState(Protocol::ENetworkEntityState networkEntityState) const;

	uint64 GetNetworkEntityId() const { return (m_spNetworkEntityInfo->entity_id()); }
	Protocol::ENetworkEntityState GetNetworkEntityState() const { return (m_spNetworkEntityInfo->entity_state()); }

	bool IsCreatedNetworkEntityInfo() const { return (m_spNetworkEntityInfo != nullptr); }

private:
	void SyncToServer_NetworkEntityLookAtDirectionIfNeed();
	void SyncToServer_NetworkEntityMoveIfNeed();
	void SyncToServer_NetworkEntityAttackIfNeed();

private:
	std::unordered_map<uint32, uint32> m_mapNetworkEntityState;
	std::unordered_map<uint32, PawnActorStatePtr> m_mapActorState;
	std::shared_ptr<Protocol::NetworkEntityInfo> m_spNetworkEntityInfo;
};