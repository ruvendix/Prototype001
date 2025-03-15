// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameEntityActor : public PawnActor
{
	using Super = PawnActor;

public:
	using Super::Super;
	virtual ~GameEntityActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void ProcessMoveDirection(const Vector2d& vMoveDir);

public:
	template <typename TActorState>
	void RegisterActorState(Protocol::EGameEntityState gameEntityState)
	{
		auto foundIter = m_mapActorState.find(TO_NUM(gameEntityState));
		if (foundIter != m_mapActorState.cend())
		{
			return;
		}

		auto insertedIter = m_mapActorState.insert(std::make_pair(TO_NUM(gameEntityState), std::make_shared<TActorState>(this)));
		ASSERT_LOG(insertedIter.second == true);
	}

public:
	void SyncFromServer_GameEntityInfo(const Protocol::GameEntityInfo& gameEntityInfo);
	void SyncFromServer_GameEntityLookAtDirection(const Protocol::GameEntityInfo& gameEntityInfo);
	void SyncFromServer_GameEntityMove(const Protocol::GameEntityInfo& gameEntityInfo);

	void SyncToServer_GameEntityInfoIfNeed();

	EActorLookAtDirection ConvertGameEntityLookAtDirectionToActorLookAtDirection(Protocol::EGameEntityLookAtDir gameEntityLookAtDir) const;
	Protocol::EGameEntityLookAtDir ConvertActorLookAtDirectionToGameEntityLookAtDirection(EActorLookAtDirection actorLookAtDir) const;

	void ApplyActorStateFromServer(Protocol::EGameEntityState gameEntityState);
	void ApplyActorLookAtDirectionFromServer(Protocol::EGameEntityLookAtDir gameEntityLookAtDir);

	bool CheckClientAndServerIsSameLookAtDirection() const;

	uint64 GetGameEntityId() const { return (m_spGameEntityInfo->entity_id()); }

private:
	void SyncToServer_GameEntityLookAtDirectionIfNeed();
	void SyncToServer_GameEntityMoveIfNeed();

private:
	std::unordered_map<uint32, PawnActorStatePtr> m_mapActorState;
	std::shared_ptr<Protocol::GameEntityInfo> m_spGameEntityInfo;
};