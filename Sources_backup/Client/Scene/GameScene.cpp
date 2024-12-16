// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "Client/Player/PlayerActor.h"
#include "Client/Player/PlayerImplementation.h"

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

	// 맵 로딩
	ResourceMananger::I()->LoadTexture("Assets/Texture/Map/Stage01.bmp");

	// 플레이어 액터 추가
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