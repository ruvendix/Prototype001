#pragma once

#include "Types.h"
#include "IDefault.h"
#include "Sprite.h"

struct TileMapInfo;

class Tile : public IDefault
{
public:
	DEFINE_SMART_PTR(Tile);

public:
	Tile();
	virtual ~Tile();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

	void SetTileSprite(Sprite::Ptr spTileSprite) { m_spSprite = spTileSprite; }
	void SetTileMapInfo(const TileMapInfo* pTileMapInfo) { m_pTileMapInfo = pTileMapInfo; }
	void SetSpriteIndex(uint32 spriteIdx) { m_spriteIdx = spriteIdx; }

	const Point2d& GetPosition() const { return m_pos; }
	void SetPosition(const Point2d& pos) { m_pos = pos; }

private:
	// Ÿ�ϸ��� ��������� ����
	const TileMapInfo* m_pTileMapInfo = nullptr;

	Point2d     m_pos;
	Sprite::Ptr m_spSprite = nullptr;
	uint32      m_spriteIdx = 0;
	bool        m_bDebugOn = true;
};