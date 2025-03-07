// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldTileActor : public CellActor
{
	using Super = CellActor;

public:
	using Super::Super;
	virtual ~WorldTileActor() override;

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void SaveToFileStream(const FileStream& fileStream) const override;
	virtual void LoadFromFileStream(const FileStream& fileStream) override;

	void InitializeWorldTile(int32 cellX, int32 cellY, const std::string& strWorldTileSpritePath);
	void LoadWorldTileFromFileStream(const FileStream& fileStream, int32 cellX, int32 cellY, const std::string& strWorldTileSpritePath);

	void ApplyNextWorldTileShape();
	void ApplyCurrentWorldTileShape();

	const std::string& BringTileSpritePath() const;

	int32 GetTileShapeIdx() const { return m_tileShapeIdx; }

private:
	int32 m_tileShapeIdx = 0;
	StaticSpritePtr m_spTileSprite = nullptr;
};