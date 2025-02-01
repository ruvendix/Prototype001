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

#pragma region ����� ó��
	// ����� ���� �߰�
	m_spWorldBackgroundActor = CreateActorToScene<WorldBackgroundActor>();

	// �� ���� �ֱ�
	WorldContext::I()->SetCellSize(48);

	// �� ������ ���� Ÿ�ϸ� ����
	m_spWorldTileMapActor = CreateActorToScene<WorldTileMapActor>();
#pragma endregion

	m_spEditorActor = CreateActorToScene<EditorActor>();
	m_spEditorActor->SetWorldTileMapActor(m_spWorldTileMapActor);

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
}