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

	// ī�޶� ����ϰ� �� �������� ���� ī�޶� Ÿ������ ����
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