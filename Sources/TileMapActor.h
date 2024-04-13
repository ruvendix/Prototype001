#pragma once

#include "Actor.h"

class TileMap;

class TileMapActor : public Actor
{
public:
	using Super = Actor;

public:
	using Actor::Actor;
	virtual ~TileMapActor() = default;

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;

private:
	TileMapPtr m_spTileMap;
};