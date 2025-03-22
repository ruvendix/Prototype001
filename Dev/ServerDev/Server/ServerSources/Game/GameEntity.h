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
	void ApplyGameEntityLookAtDirection(const Protocol::GameEntityInfo& gameEntityInfo);
	void ApplyGameEntityMoveInfo(const Protocol::GameEntityInfo& gameEntityInfo);
	void ApplyGameEntityState(const Protocol::GameEntityInfo& gameEntityInfo);

	bool CheckSameCellPosition(const Position2d& targetCellPos) const;
	Position2d MakeCurrentCellPosition() const;

	Protocol::EGameEntityLookAtDir CalculateGameEntityLookAtDirection(const Position2d& destCellPos);
	Protocol::EGameEntityLookAtDir CalculateGameEntityLookAtDirection(const Protocol::GameEntityInfo& modifiedGameEntityInfo);

	void SetGameEntityInfo(const Protocol::GameEntityInfo& gameEntityInfo) { m_gameEntityInfo = gameEntityInfo; }

	Protocol::GameEntityInfo& GetGameEntityInfo() { return m_gameEntityInfo; }
	const Protocol::GameEntityInfo& GetGameEntityInfo() const { return m_gameEntityInfo; }

private:
	Protocol::GameEntityInfo m_gameEntityInfo;
};