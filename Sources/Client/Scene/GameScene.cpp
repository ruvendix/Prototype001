// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameScene.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "Client/World/WorldBackgroundActor.h"
#include "Client/Player/PlayerActor.h"

namespace
{
	int32 g_editWorldTileInputMappingContextId = -1;
}

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

#pragma region �Է� ó��
	// �Է°� �����
	InputActionValue inputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spInputAction = std::make_unique<InputAction>("EditWorldTile");
	spInputAction->ApplyInputActionValue(inputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo leftMouseButtonInputMappingInfo;
	leftMouseButtonInputMappingInfo.inputValue = EInputValue::MouseLeftButton;
	leftMouseButtonInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spInputAction->AddInputMappingInfo(leftMouseButtonInputMappingInfo);

	// ���� ���ؽ�Ʈ �׽�Ʈ
	InputMappingContextPtr spEditWorldTileInputMappingContext = std::make_shared<InputMappingContext>();
	spEditWorldTileInputMappingContext->AddInputAction(spInputAction);
	g_editWorldTileInputMappingContextId = spEditWorldTileInputMappingContext->GetId();

	// �Է� �ý����� �ٽ�! �Է� ����̽����� ������
	LocalPlayerInputSystem::I()->AddInputMappingContext(spEditWorldTileInputMappingContext, 0);

	// ������ ĸó ����� �̿��� �ڵ鷯 ��� ���
	LocalPlayerInputSystem::I()->BindInputActionHandler(spInputAction, this, &GameScene::OnMouseLeftButtonHandler);
#pragma endregion

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
	LocalPlayerInputSystem::I()->RemoveInputMappingContext(g_editWorldTileInputMappingContextId);
}

void GameScene::OnMouseLeftButtonHandler(const InputActionValue* pInputAction)
{
	// ���� ���콺 ��ǥ�� Ÿ�Ͽ� ����
	m_spWorldTileMapActor->ApplyCurrentMousePositionToTile();
	DEFAULT_TRACE_LOG("���콺 ���� ��ư");
}