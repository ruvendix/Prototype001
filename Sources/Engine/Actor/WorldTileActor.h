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
	void ApplyNextShape();

	int32 GetTileShapeIdx() const { return m_tileShapeIdx; }

private:
	int32 m_tileShapeIdx = 0;
	StaticSpritePtr m_spTileSprite = nullptr;
};