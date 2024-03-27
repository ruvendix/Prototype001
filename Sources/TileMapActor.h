#pragma once

#include "Actor.h"
#include "Tile.h"

struct TileMapInfo
{
	Size   tileSize;
	uint2d totalTileCount;
};

class TileMapActor : public Actor
{
public:
	DEFINE_SMART_PTR(TileMapActor);
	
public:
	using Actor::Actor;
	virtual ~TileMapActor() = default;

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;

private:
	using TileMapLine = std::vector<Tile::Ptr>;
	std::vector<TileMapLine> m_tileMap;
	TileMapInfo m_tileMapInfo;
};