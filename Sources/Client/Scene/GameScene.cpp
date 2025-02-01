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

}

void GameScene::Startup()
{
	Super::Startup();

#pragma region 월드맵 처리
	// 월드맵 액터 추가
	m_spWorldBackgroundActor = CreateActorToScene<WorldBackgroundActor>();

	// 셀 정보 넣기
	WorldContext::I()->SetCellSize(48);

	// 셀 정보에 따른 타일맵 구성
	m_spWorldTileMapActor = CreateActorToScene<WorldTileMapActor>();
#pragma endregion

	// 플레이어 액터 추가
	m_spPlayerActor = CreateActorToScene<PlayerActor>();
	m_spPlayerActor->SetWorldTileMapActor(m_spWorldTileMapActor);

	// 카메라 등록하고 씬 렌더러의 메인 카메라 타겟으로 설정
	RegisterMainCameraActorToScene(m_spPlayerActor);
}

bool GameScene::Update(float deltaSeconds)
{
	return (Super::Update(deltaSeconds));
}

void GameScene::Cleanup()
{
	Super::Cleanup();
}