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
	Protocol::EGameEntityLookAtDir CalculateActorLookAtDirection(const Protocol::GameEntityInfo& modifiedGameEntityInfo);

	void SetGameEntityInfo(const Protocol::GameEntityInfo& gameEntityInfo) { m_gameEntityInfo = gameEntityInfo; }
	const Protocol::GameEntityInfo& GetGameEntityInfo() const { return m_gameEntityInfo; }

private:
	Protocol::GameEntityInfo m_gameEntityInfo;
};