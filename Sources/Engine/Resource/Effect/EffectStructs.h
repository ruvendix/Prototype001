// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

struct EffectSpawnInfo
{
	std::string strEffectName;
	Position2d spawnCellPos;
	Size effectSize;
	float lifeTime = 0.0f;
	ActorPtr spTargetActor = nullptr;
};