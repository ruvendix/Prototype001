#pragma once

#include "ResourceBase.h"

class TileMap : public ResourceBase
{
public:
	using TileLine = std::vector<TilePtr>;
	using Tiles = std::vector<TileLine>;

public:
	TileMap(const std::string& strResPath);
	virtual ~TileMap();

	virtual bool Update()  override;
	virtual void Render()  override;

	void SaveResource() override;
	void LoadResource() override;

	const Size&   GetTileSize() const { return m_tileSize; }
	const uint2d& GetTotalTileCount() const { return m_totalTileCount; }
	const Tiles&  GetTiles() const { return m_tiles; };

private:
	Tiles  m_tiles;
	Size   m_tileSize;
	uint2d m_totalTileCount;
};