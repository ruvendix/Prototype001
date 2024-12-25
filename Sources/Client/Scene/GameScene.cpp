// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "Client/World/WorldBackgroundActor.h"
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

	// 월드맵 액터 추가
	m_pWorldBackgroundActor = CreateActor<WorldBackgroundActor>();

	// 셀 정보 넣기
	WorldContext::I()->SetCellSize(48);

	// 셀 정보에 따른 타일맵 구성


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