// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Actor.h"

class CellActor : public Actor
{
public:
	using Super = Actor;

public:
	static Vec2d ConvertCellPositionToWorldPosition(const CellPosition& cellPos);
	static Vec2d ConvertCellPositionToWorldPosition(int32 cellPosX, int32 cellPosY);
	static CellPosition ConvertWorldPositionToCellPosition(const Vec2d& worldPos);
	static CellPosition ConvertWorldPositionToCellPosition(float posX, float posY);

public:
	using Super::Super;
	virtual ~CellActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	void ApplyCellPosition(int32 cellPosX, int32 cellPosY);
	const CellPosition& GetCellPosition() const { return m_cellPos; }

private:
	CellPosition m_cellPos;
};