// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EditScene.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "Client/World/WorldBackgroundActor.h"
#include "Client/Editor/EditorActor.h"

EditScene::EditScene()
{

}

EditScene::~EditScene()
{

}

void EditScene::Startup()
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

	m_spEditorActor = CreateActorToScene<EditorActor>();
	m_spEditorActor->SetWorldTileMapActor(m_spWorldTileMapActor);

	// 카메라 등록하고 씬 렌더러의 메인 카메라 타겟으로 설정
	RegisterMainCameraActorToScene(m_spEditorActor);
}

bool EditScene::Update(float deltaSeconds)
{
	return (Super::Update(deltaSeconds));
}

void EditScene::Cleanup()
{
	Super::Cleanup();
}