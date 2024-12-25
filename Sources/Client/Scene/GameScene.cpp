// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "Engine/Actor/WorldTileMapActor.h"
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
	m_spWorldBackgroundActor = CreateActorToScene<WorldBackgroundActor>();

	// 셀 정보 넣기
	WorldContext::I()->SetCellSize(48);

	// 셀 정보에 따른 타일맵 구성
	m_spWorldTileMapActor = CreateActorToScene<WorldTileMapActor>();

	// 플레이어 액터 추가
	m_spPlayerActor = CreateActorToScene<PlayerActor>();

	// 씬 렌더러의 메인 카메라 타겟으로 설정
	SceneRenderer::I()->ApplyTargetActorToMainCamera(m_spPlayerActor);
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