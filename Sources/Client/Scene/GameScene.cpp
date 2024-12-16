// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "Client/Player/PlayerActor.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	Cleanup();
}

void GameScene::Startup()
{
	Super::Startup();

#pragma region ����� ���� ���� (�̰� ��� ���� ����غ��� ���Ϻ��� �Ǵ�)
	const std::string& strWorldMapTexPath = "Assets/Texture/Map/Stage01.bmp";
	TexturePtr spWorldMapTex = ResourceMananger::I()->LoadTexture(strWorldMapTexPath);
	const Size& worldMapTexSize = spWorldMapTex->GetTextureSize();

	const StaticSpritePtr& spWorldMapSprite = ResourceMananger::I()->CreateStaticSprite("WorldMap");
	ASSERT_LOG(spWorldMapSprite != nullptr);
	spWorldMapSprite->FindAndSetTexture(strWorldMapTexPath);

	// ������ ���� ��ġ!
	spWorldMapSprite->SetDrawSize(worldMapTexSize);

	Actor* pWorldMapActor = CreateActor<Actor>();
	pWorldMapActor->BringTransformComponent()->SetPosition(worldMapTexSize.width / 2.0f, worldMapTexSize.height / 2.0f);
	pWorldMapActor->BringTransformComponent()->SetSize(worldMapTexSize);

	// ��������Ʈ ����
	StaticSpriteComponent* pStaticSpriteComponent = pWorldMapActor->AddComponent<StaticSpriteComponent>();
	ASSERT_LOG(pStaticSpriteComponent != nullptr);
	pStaticSpriteComponent->SetStaticSprite(spWorldMapSprite);
#pragma endregion

	// �÷��̾� ���� �߰�
	m_pPlayerActor = CreateActor<PlayerActor>();
}

bool GameScene::Update(float deltaSeconds)
{
	return (Super::Update(deltaSeconds));
}

bool GameScene::PostUpdate(float deltaSeconds)
{
	return (Super::PostUpdate(deltaSeconds));
}

void GameScene::Cleanup()
{
	return (Super::Cleanup());
}