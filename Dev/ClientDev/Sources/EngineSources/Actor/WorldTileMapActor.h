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

	virtual void SaveToFileStream(const FileStream& fileStream) const override;
	virtual void LoadFromFileStream(const FileStream& fileStream) override;

	void ApplyCurrentMousePositionToTile();
	bool CheckMovingAvailableTile(const Position2d& cellPos) const;

	int32 ConvertCellPositionToTileIndex(const Position2d& cellPos) const;
	void ShowWorldTileGuideShape(bool bShow);

private:
	Array2dInfo m_tileArray2dInfo;
};