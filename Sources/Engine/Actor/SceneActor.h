// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Actor.h"

class SceneActor : public Actor
{
public:
	using Super = Actor;

public:
	using Super::Super;
	virtual ~SceneActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	void ApplyCellPosition(int32 cellPosX, int32 cellPosY);
	
	Vec2d ConvertCellPositionToWorldPosition(const CellPosition& cellPos) const;
	Vec2d ConvertCellPositionToWorldPosition(int32 cellPosX, int32 cellPosY) const;
	CellPosition ConvertWorldPositionToCellPosition(float posX, float posY) const;

	const CellPosition& GetCellPosition() const { return m_cellPos; }

private:
	CellPosition m_cellPos;
};