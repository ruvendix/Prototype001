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

	m_spEditorActor = CreateActorToScene<EditorActor>(EActorLayerType::Default);
	m_spEditorActor->SetActorFlagBitOff(EActorFlag::RenderingTarget);
	m_spEditorActor->SetWorldTileMapActor(GetWorldTileMapActor());

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