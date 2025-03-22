// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

namespace GameMonsterUtility
{
	bool CheckValidDistance(const Position2d& srcCellPos, const Position2d& destCellPos, uint32 dist);
	GamePlayerPtr FindNearestPlayer(const GameMonsterPtr& spMonster, uint32 dist);
	bool CalculateNavigationPath(const Position2d& srcCellPos, const Position2d& destCellPos, int32 chaseRange, std::vector<Position2d>& outVecNavigationPath);
}