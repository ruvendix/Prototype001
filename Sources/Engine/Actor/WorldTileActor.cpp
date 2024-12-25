// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WorldTileActor.h"

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
	const StaticSpritePtr& spWorldTileSprite = ResourceMananger::I()->CreateStaticSprite(strWorldTileTag);
	ASSERT_LOG(spWorldTileSprite != nullptr);

	const std::string& strWorldTileTexPath = "Assets/Texture/Map/Tile.bmp";
	spWorldTileSprite->LoadAndSetTexture(strWorldTileTexPath);

	SpriteDrawInfo worldTileSpriteDrawInfo;
	worldTileSpriteDrawInfo.drawSize = Size{ cellSize, cellSize };
	worldTileSpriteDrawInfo.colorKey = RGB(128, 128, 128);
	spWorldTileSprite->SetSpriteDrawInfo(worldTileSpriteDrawInfo);

	// 스프라이트 설정
	StaticSpriteComponent* pStaticSpriteComponent = AddComponent<StaticSpriteComponent>();
	ASSERT_LOG(pStaticSpriteComponent != nullptr);
	pStaticSpriteComponent->SetStaticSprite(spWorldTileSprite);
}