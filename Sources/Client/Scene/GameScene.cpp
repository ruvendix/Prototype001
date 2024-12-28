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

#pragma region ����� ó��
	// ����� ���� �߰�
	m_spWorldBackgroundActor = CreateActorToScene<WorldBackgroundActor>();

	// �� ���� �ֱ�
	WorldContext::I()->SetCellSize(48);

	// �� ������ ���� Ÿ�ϸ� ����
	m_spWorldTileMapActor = CreateActorToScene<WorldTileMapActor>();
#pragma endregion

	// �÷��̾� ���� �߰�
	m_spPlayerActor = CreateActorToScene<PlayerActor>();
	m_spPlayerActor->SetWorldTileMapActor(m_spWorldTileMapActor);

	// �� �������� ���� ī�޶� Ÿ������ ����
	SceneRenderer::I()->ApplyTargetActorToMainCamera(m_spPlayerActor);

	// �Է� ó��
	Callback mouseLeftButtonDownCallback = std::bind(&GameScene::OnMouseLeftButtonDown, this);
	MouseInputHandler::I()->BindMouseInput(EMouseValue::LeftButton, mouseLeftButtonDownCallback, nullptr, nullptr);
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

void GameScene::OnMouseLeftButtonDown()
{
	// ���� ���콺 ��ǥ�� Ÿ�Ͽ� ����
	m_spWorldTileMapActor->ApplyCurrentMousePositionToTile();
	DEFAULT_TRACE_LOG("���콺 ���� ��ư");
}