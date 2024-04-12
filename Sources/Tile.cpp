#include "Pch.h"
#include "Tile.h"

#include "TileMapActor.h"
#include "GameApplication.h"
#include "Camera.h"

Tile::Tile()
{

}

Tile::~Tile()
{

}

void Tile::Startup()
{

}

bool Tile::Update()
{
	return true;
}

bool Tile::PostUpdate()
{
	return true;
}

void Tile::Render()
{
	const Size& tileSize = m_pTileMapInfo->tileSize;

	Point2d destPos;
	Camera::Ptr spCamera = GameApplication::I()->GetCurrentCamera();
	destPos.x = m_pos.x - spCamera->GetOffsetPosition().x;
	destPos.y = m_pos.y - spCamera->GetOffsetPosition().y;

	const SpriteInfo* pSpriteInfo = m_spSprite->GetSpriteInfo(m_spriteIdx);

	::TransparentBlt
	(
		GameApplication::I()->GetBackBufferDc(),
		destPos.x,
		destPos.y,
		tileSize.width,
		tileSize.height,
		pSpriteInfo->spTex->GetDc(),
		pSpriteInfo->pos.x,
		pSpriteInfo->pos.y,
		pSpriteInfo->size.width,
		pSpriteInfo->size.height,
		pSpriteInfo->excludeColor
	);
}

void Tile::Cleanup()
{

}