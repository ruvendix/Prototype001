// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileMapActor : public Actor
{
	using Super = Actor;

public:
	using Super::Super;
	virtual ~WorldTileMapActor() override;

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	void ApplyCurrentMousePositionToTile();
	bool CheckMovingAvailableTile(const CellPosition& cellPos) const;

	int32 ConvertCellPositionToTileIndex(const CellPosition& cellPos) const;

private:
	Array2dInfo m_tileArray2dInfo;
};