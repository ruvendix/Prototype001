#pragma once

#include "Actor.h"

struct TileMapInfo
{
	Size tileSize;
	uint2d totalTileCount;
};

class TileMapActor : public Actor
{
public:
	using Actor::Actor;
	virtual ~TileMapActor() = default;

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;

private:
	using TileMapLine = std::vector<TilePtr>;
	std::vector<TileMapLine> m_tileMap;
	TileMapInfo m_tileMapInfo;
};