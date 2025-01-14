// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Actor.h"

class CellActor : public Actor
{
public:
	using Super = Actor;

public:
	static Vec2d ConvertCellPositionToWorldPosition(const Position2d& cellPos);
	static Vec2d ConvertCellPositionToWorldPosition(int32 x, int32 y);
	static Position2d ConvertWorldPositionToCellPosition(const Vec2d& worldPos);
	static Position2d ConvertWorldPositionToCellPosition(float posX, float posY);

public:
	using Super::Super;
	virtual ~CellActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	void ApplyCellPosition(int32 x, int32 y);
	const Position2d& GetCellPosition() const { return m_cellPos; }

private:
	Position2d m_cellPos;
};