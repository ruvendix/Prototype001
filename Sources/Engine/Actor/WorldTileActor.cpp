// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WorldTileActor.h"

namespace
{
	const int32 TILE_SHAPE_COUNT = 4;
}

WorldTileActor::~WorldTileActor()
{

}

void WorldTileActor::Startup()
{
	Super::Startup();
}

bool WorldTileActor::PostUpdate(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	const std::shared_ptr<CameraActor>& spMainCameraActor = SceneRenderer::I()->GetMainCameraActor();
	if (spMainCameraActor->CheckInCameraVision(this) == true)
	{
		SetActorFlagBitOn(EActorFlag::RenderingTarget);
	}
	else
	{
		SetActorFlagBitOff(EActorFlag::RenderingTarget);
	}

	return true;
}

void WorldTileActor::Cleanup()
{
	Super::Cleanup();
}

void WorldTileActor::InitializeWorldTile(int32 cellX, int32 cellY)
{
	ApplyCellPosition(cellX, cellY);

	std::string strWorldTileTag;
	MakeFormatString(strWorldTileTag, "WorldTile(%d, %d)", cellX, cellY);

	int32 cellSize = WorldContext::I()->GetCellSize();

	TransformComponent* pWorldTileTransform = BringTransformComponent();
	pWorldTileTransform->SetSize(cellSize, cellSize);

	// 이미지 넣기
	m_spTileSprite = ResourceMananger::I()->CreateStaticSprite(strWorldTileTag);
	ASSERT_LOG(m_spTileSprite != nullptr);

	const std::string& strWorldTileTexPath = "Assets/Texture/Map/Tile.bmp";
	m_spTileSprite->LoadAndSetTexture(strWorldTileTexPath);

	SpriteDrawInfo worldTileSpriteDrawInfo;
	worldTileSpriteDrawInfo.drawSize = Size{ cellSize, cellSize };
	worldTileSpriteDrawInfo.colorKey = RGB(128, 128, 128);
	m_spTileSprite->SetSpriteDrawInfo(worldTileSpriteDrawInfo);

	// 스프라이트 설정
	StaticSpriteComponent* pStaticSpriteComponent = AddComponent<StaticSpriteComponent>();
	ASSERT_LOG(pStaticSpriteComponent != nullptr);
	pStaticSpriteComponent->SetStaticSprite(m_spTileSprite);

	// 레이어 처리
	SetRenderingLayer(ERenderingLayerType::WorldTile);
}

void WorldTileActor::ApplyNextShape()
{
	m_tileShapeIdx = global::CirculateIndex(m_tileShapeIdx, TILE_SHAPE_COUNT);

	Position2d tileDrawingStartPos;
	tileDrawingStartPos.x = (m_tileShapeIdx * WorldContext::I()->GetCellSize());
	tileDrawingStartPos.y = 0;

	m_spTileSprite->SetDrawingStartPos(tileDrawingStartPos);
}