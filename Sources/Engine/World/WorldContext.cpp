// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WorldContext.h"

DEFINE_SINGLETON(WorldContext);

Position2d WorldContext::CalculateRandomCellPosition() const
{
    int32 widthCellCount = (m_worldSize.width / m_cellSize);
    int32 heightCellCount = (m_worldSize.height / m_cellSize);

    Position2d randomCellPos;
    randomCellPos.x = (std::rand() % widthCellCount);
    randomCellPos.y = (std::rand() % heightCellCount);

    return randomCellPos;
}