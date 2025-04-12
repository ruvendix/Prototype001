// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameEntity : public EnableSharedClass
{
public:
	GameEntity();
	virtual ~GameEntity();

	virtual void Startup() override;
	virtual void Cleanup() override;

	virtual void AddToSyncEntitiesPacket(Protocol::S_SyncEntitiesPacket& outSyncEntitiesPacket);

public:
	void ApplyGameEntityLookAtDirection(const Protocol::EntityInfo& entityInfo);
	void ApplyGameEntityMoveInfo(const Protocol::EntityInfo& entityInfo);
	void ApplyGameEntityState(Protocol::EEntityState entityState);
	void ApplyGameEntityHp(int32 entityHp);

	bool CheckGameEntityState(Protocol::EEntityState entityState) const;
	bool CheckSameCellPosition(const Position2d& targetCellPos) const;
	
	Position2d MakeCurrentCellPosition() const;
	Position2d MakeForwardCellPosition() const;

	Protocol::EEntityLookAtDirection CalculateGameEntityLookAtDirection(const Position2d& destCellPos) const;
	Protocol::EEntityLookAtDirection CalculateGameEntityLookAtDirection(const Protocol::EntityInfo& targetEntityInfo) const;

	void SetGameEntityInfo(const Protocol::EntityInfo& entityInfo) { m_entityInfo = entityInfo; }

	Protocol::EntityInfo& GetGameEntityInfo() { return m_entityInfo; }
	const Protocol::EntityInfo& GetGameEntityInfo() const { return m_entityInfo; }

private:
	Protocol::EntityInfo m_entityInfo;
};