// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldContext
{
	DECLARE_SINGLETON(WorldContext)

public:
	void SetWorldSize(const Size& worldSize) { m_worldSize = worldSize; }
	const Size& GetWorldSize() const { return m_worldSize; }
	
	void SetCellSize(int32 cellSize) { m_cellSize = cellSize; }
	int32 GetCellSize() const { return m_cellSize; }

	Position2d CalculateRandomCellPosition() const;

private:
	Size m_worldSize;
	int32 m_cellSize = 0; // 셀은 정사각형임
};