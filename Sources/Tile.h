#pragma once

#include "IGameDefault.h"

class TileMap;

class Tile : public IGameDefault
{
public:
	Tile(const TileMap* pOwner);
	virtual ~Tile();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	SpritePtr GetTileSprite() const { return m_spSprite; }
	void SetTileSprite(SpritePtr spTileSprite) { m_spSprite = spTileSprite; }

	uint32 GetTileSpriteIndex() const { return m_tileSpriteIdx; }
	void SetTileSpriteIndex(uint32 tileSpriteIdx) { m_tileSpriteIdx = tileSpriteIdx; }

	const Int2d& GetPosition() const { return m_pos; }
	void SetPosition(const Int2d& pos) { m_pos = pos; }

private:
	// 타일맵을 참고용으로 가짐
	const TileMap* m_pOwner = nullptr;

	SpritePtr m_spSprite = nullptr;
	uint32    m_tileSpriteIdx = 0;

	Int2d m_pos;
	bool m_bDebugOn = true;
};