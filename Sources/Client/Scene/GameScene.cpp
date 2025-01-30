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

#pragma region 입력 처리
	// 입력값 저장용
	InputActionValue inputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spInputAction = std::make_unique<InputAction>("EditWorldTile");
	spInputAction->ApplyInputActionValue(inputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo leftMouseButtonInputMappingInfo;
	leftMouseButtonInputMappingInfo.inputValue = EInputValue::MouseLeftButton;
	leftMouseButtonInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spInputAction->AddInputMappingInfo(leftMouseButtonInputMappingInfo);

	// 매핑 컨텍스트 테스트
	InputMappingContextPtr spEditWorldTileInputMappingContext = std::make_shared<InputMappingContext>();
	spEditWorldTileInputMappingContext->AddInputAction(spInputAction);
	g_editWorldTileInputMappingContextId = spEditWorldTileInputMappingContext->GetId();

	// 입력 시스템이 핵심! 입력 디바이스보다 하위임
	LocalPlayerInputSystem::I()->AddInputMappingContext(spEditWorldTileInputMappingContext, 0);

	// 람다의 캡처 기능을 이용한 핸들러 등록 방식
	LocalPlayerInputSystem::I()->BindInputActionHandler(spInputAction, this, &GameScene::OnMouseLeftButtonHandler);
#pragma endregion

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
	LocalPlayerInputSystem::I()->RemoveInputMappingContext(g_editWorldTileInputMappingContextId);
}

void GameScene::OnMouseLeftButtonHandler(const InputActionValue* pInputAction)
{
	// 현재 마우스 좌표를 타일에 적용
	m_spWorldTileMapActor->ApplyCurrentMousePositionToTile();
	DEFAULT_TRACE_LOG("마우스 왼쪽 버튼");
}