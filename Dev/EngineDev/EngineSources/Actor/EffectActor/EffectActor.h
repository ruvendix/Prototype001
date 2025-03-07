// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/Resource/Effect/EffectStructs.h"

class EffectActor : public CellActor
{
	using Super = CellActor;

public:
	EffectActor();
	virtual ~EffectActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	void SpawnEffect(const EffectSpawnInfo& effectSpawnInfo);
};