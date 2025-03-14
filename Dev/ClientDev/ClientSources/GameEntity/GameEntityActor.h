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
	void SyncFromServer_GameEntityInfo(const Protocol::GameEntityInfo& gameEntityInfo);
	void SyncFromServer_GameEntityMove(const Protocol::GameEntityInfo& gameEntityInfo);

	void SyncToServer_GameEntityInfoIfNeed();

	uint64 GetGameEntityId() const { return (m_spGameEntityInfo->entity_id()); }

private:
	void SyncToServer_GameEntityMoveIfNeed();

private:
	std::shared_ptr<Protocol::GameEntityInfo> m_spGameEntityInfo;
};