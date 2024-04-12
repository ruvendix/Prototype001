#pragma once

#include "Types.h"
#include "IGameDefault.h"
#include "Sprite.h"

struct TileMapInfo;

class Tile : public IGameDefault
{
public:
	DEFINE_SMART_PTR(Tile);

public:
	Tile();
	virtual ~Tile();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void SetTileSprite(Sprite::Ptr spTileSprite) { m_spSprite = spTileSprite; }
	void SetTileMapInfo(const TileMapInfo* pTileMapInfo) { m_pTileMapInfo = pTileMapInfo; }
	void SetSpriteIndex(uint32 spriteIdx) { m_spriteIdx = spriteIdx; }

	const Point2d& GetPosition() const { return m_pos; }
	void SetPosition(const Point2d& pos) { m_pos = pos; }

private:
	// 타일맵을 참고용으로 가짐
	const TileMapInfo* m_pTileMapInfo = nullptr;

	Point2d     m_pos;
	Sprite::Ptr m_spSprite = nullptr;
	uint32      m_spriteIdx = 0;
	bool        m_bDebugOn = true;
};