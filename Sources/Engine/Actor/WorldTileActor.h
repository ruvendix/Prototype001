// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileActor : public CellActor
{
	using Super = CellActor;

public:
	using Super::Super;
	virtual ~WorldTileActor() override;

	virtual void Startup() override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	void InitializeWorldTile(int32 cellX, int32 cellY);
};