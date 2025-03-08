// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EditScene.h"

#include "EngineSources/Actor/WorldTileMapActor.h"
#include "ClientSources/World/WorldBackgroundActor.h"
#include "ClientSources/Editor/EditorActor.h"

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

	// ī�޶� ����ϰ� �� �������� ���� ī�޶� Ÿ������ ����
	RegisterMainCameraActorToScene(m_spEditorActor);
}

bool EditScene::Update(float deltaSeconds)
{
	return (Super::Update(deltaSeconds));
}

void EditScene::Cleanup()
{
	Super::Cleanup();
	SceneRenderer::I()->SetMainCameraActor(nullptr);
}