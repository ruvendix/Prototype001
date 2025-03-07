// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Actor.h"

class CellActor : public Actor
{
public:
	using Super = Actor;

public:
	static Vector2d ConvertCellPositionToWorldPosition(const Position2d& cellPos);
	static Vector2d ConvertCellPositionToWorldPosition(int32 x, int32 y);
	static Position2d ConvertWorldPositionToCellPosition(const Vector2d& worldPos);
	static Position2d ConvertWorldPositionToCellPosition(float posX, float posY);

public:
	using Super::Super;
	virtual ~CellActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual bool CheckMovingState() const;

	void ApplyCellPosition(int32 x, int32 y);
	void ApplyCellPosition(const Position2d& cellPos);
	Position2d ApplyRandomCellPosition();

	Vector2d CalculateMoveDirectionByCellPosition(const Position2d& destCellPos) const;

	void SetCellPosition(const Position2d& cellPos) { m_cellPos = cellPos; }
	const Position2d& GetCellPosition() const { return m_cellPos; }

	bool CheckEqaulCellPosition(const Position2d& otherCellPos) const;

private:
	Position2d m_cellPos;
};