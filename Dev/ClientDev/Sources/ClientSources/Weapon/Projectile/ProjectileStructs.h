// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

struct ProjectileSpawnInfo
{
	int32 projectileId = -1;
	Position2d spawnCellPos;
	Size projectileSize;
	Vector2d vMoveDir;
	float flyingSpeed = 0.0f;
};