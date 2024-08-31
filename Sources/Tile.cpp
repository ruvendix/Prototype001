#include "Pch.h"
#include "Tile.h"

#include "Sprite.h"
#include "Texture.h"
#include "TileMap.h"
#include "GameApplication.h"
#include "Camera.h"
#include "BoxCollider.h"

Tile::Tile(const TileMap* pOwner)
{
	m_pOwner = pOwner;
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
	const Size& tileSize = m_pOwner->GetTileSize();

	Int2d destPos;
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();
	destPos.x = m_pos.x - spCamera->GetOffsetPosition().x;
	destPos.y = m_pos.y - spCamera->GetOffsetPosition().y;

	const SpriteInfo* pSpriteInfo = m_spSprite->GetSpriteInfo(m_tileSpriteIdx);

	::TransparentBlt
	(
		GameApplication::I()->GetBackBufferDc(),
		destPos.x,
		destPos.y,
		tileSize.width,
		tileSize.height,
		pSpriteInfo->spTex->GetDc(),
		pSpriteInfo->startPos.x,
		pSpriteInfo->startPos.y,
		pSpriteInfo->size.width,
		pSpriteInfo->size.height,
		pSpriteInfo->excludeColor
	);
}

void Tile::Cleanup()
{

}