// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameEntity : public EnableSharedClass
{
public:
	GameEntity();
	virtual ~GameEntity();

	virtual void Startup() override;
	virtual void Cleanup() override;

public:
	void ApplyGameEntityLookAtDirection(const Protocol::NetworkEntityInfo& entityInfo);
	void ApplyGameEntityMoveInfo(const Protocol::NetworkEntityInfo& entityInfo);
	void ApplyGameEntityState(const Protocol::NetworkEntityInfo& entityInfo);

	bool CheckGameEntityState(Protocol::ENetworkEntityState entityState) const;
	bool CheckSameCellPosition(const Position2d& targetCellPos) const;
	
	Position2d MakeCurrentCellPosition() const;
	Position2d MakeForwardCellPosition() const;

	Protocol::ENetworkEntityLookAtDirection CalculateGameEntityLookAtDirection(const Position2d& destCellPos) const;
	Protocol::ENetworkEntityLookAtDirection CalculateGameEntityLookAtDirection(const Protocol::NetworkEntityInfo& targetEntityInfo) const;

	void SetGameEntityInfo(const Protocol::NetworkEntityInfo& entityInfo) { m_entityInfo = entityInfo; }

	Protocol::NetworkEntityInfo& GetGameEntityInfo() { return m_entityInfo; }
	const Protocol::NetworkEntityInfo& GetGameEntityInfo() const { return m_entityInfo; }

private:
	Protocol::NetworkEntityInfo m_entityInfo;
};