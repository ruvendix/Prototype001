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

	// ����� ���� �߰�
	m_pWorldBackgroundActor = CreateActor<WorldBackgroundActor>();

	// �� ���� �ֱ�
	WorldContext::I()->SetCellSize(48);

	// �� ������ ���� Ÿ�ϸ� ����


	// �÷��̾� ���� �߰�
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