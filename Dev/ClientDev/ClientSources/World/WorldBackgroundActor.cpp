// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WorldBackgroundActor.h"

WorldBackgroundActor::~WorldBackgroundActor()
{

}

void WorldBackgroundActor::Startup()
{
	Super::Startup();

	const std::string& strWorldMapTexPath = "Assets/Texture/Map/Stage01.bmp";
	TexturePtr spWorldMapTex = ResourceMananger::I()->LoadTexture(strWorldMapTexPath);
	const Size& worldSize = spWorldMapTex->GetTextureSize();

	// ���� ũ��� ���⼭ ����
	WorldContext::I()->SetWorldSize(worldSize);

	const StaticSpritePtr& spWorldMapSprite = ResourceMananger::I()->CreateStaticSprite("WorldMap");
	ASSERT_LOG(spWorldMapSprite != nullptr);
	spWorldMapSprite->FindAndSetTexture(strWorldMapTexPath);

	SpriteDrawInfo worldMapSpriteDrawInfo;
	worldMapSpriteDrawInfo.drawSize = worldSize;
	spWorldMapSprite->SetSpriteDrawInfo(worldMapSpriteDrawInfo);

	TransformComponent* pTransformComponent = BringTransformComponent();
	pTransformComponent->SetPosition(worldSize.width / 2.0f, worldSize.height / 2.0f);
	pTransformComponent->SetSize(worldSize);

	// ��������Ʈ ����
	StaticSpriteComponent* pStaticSpriteComponent = AddComponent<StaticSpriteComponent>();
	ASSERT_LOG(pStaticSpriteComponent != nullptr);
	pStaticSpriteComponent->SetStaticSprite(spWorldMapSprite);
}

void WorldBackgroundActor::Cleanup()
{

}