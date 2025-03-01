// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EditorActor.h"

#include "EngineSources/Actor/WorldTileMapActor.h"
#include "ClientSources/Scene/GameScene.h"

namespace
{
	int32 g_editorActorInputMappingContextId = -1;
}

EditorActor::~EditorActor()
{
}

void EditorActor::Startup()
{
	Super::Startup();

#pragma region 입력 처리
	// 매핑 컨텍스트 추가
	InputMappingContextPtr spInputMappingContext = std::make_shared<InputMappingContext>();
	g_editorActorInputMappingContextId = spInputMappingContext->GetId();
	LocalPlayerInputSystem::I()->AddInputMappingContext(spInputMappingContext, 0);

#pragma region 마우스 왼쪽 버튼으로 편집
	// 입력값 저장용
	InputActionValue editWorldTileInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spEditWorldTileInputAction = std::make_unique<InputAction>("EditWorldTile");
	spEditWorldTileInputAction->ApplyInputActionValue(editWorldTileInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo leftMouseButtonInputMappingInfo;
	leftMouseButtonInputMappingInfo.inputValue = EInputValue::MouseLeftButton;
	leftMouseButtonInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spEditWorldTileInputAction->AddInputMappingInfo(leftMouseButtonInputMappingInfo);
	LocalPlayerInputSystem::I()->BindInputActionHandler(spEditWorldTileInputAction, this, &EditorActor::OnMouseLeftButtonHandler);

	// 입력 시스템이 핵심! 입력 디바이스보다 하위임
	spInputMappingContext->AddInputAction(spEditWorldTileInputAction);
#pragma endregion

#pragma region 마우스 오른쪽 버튼으로 게임 씬으로 전환
	// 입력값 저장용
	InputActionValue reserveGameSceneInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spReserveGameSceneInputAction = std::make_unique<InputAction>("ReserveGameScene");
	spReserveGameSceneInputAction->ApplyInputActionValue(reserveGameSceneInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo rightMouseButtonInputMappingInfo;
	rightMouseButtonInputMappingInfo.inputValue = EInputValue::MouseRightButton;
	rightMouseButtonInputMappingInfo.inputTrigger = EInputTrigger::OnlyReleased;
	spReserveGameSceneInputAction->AddInputMappingInfo(rightMouseButtonInputMappingInfo);
	LocalPlayerInputSystem::I()->BindInputActionHandler(spReserveGameSceneInputAction, this, &EditorActor::OnMouseRightButtonHandler);

	// 입력 시스템이 핵심! 입력 디바이스보다 하위임
	spInputMappingContext->AddInputAction(spReserveGameSceneInputAction);
#pragma endregion

#pragma region 방향키 이동
	// 이 경우는 이동이니까 Vector2로 해결

	// 입력값 저장용
	InputActionValue moveEditorInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spMoveEditorInputAction = std::make_unique<InputAction>("MoveEditor");
	spMoveEditorInputAction->ApplyInputActionValue(moveEditorInputActionValue, EInputActionValueType::Vector2);

#pragma region 방향키에 따른 값 설정
	InputActionMappingInfo leftKeyInputMappingInfo;
	leftKeyInputMappingInfo.inputValue = EInputValue::Left; // 왼쪽키를 누르면 작동하는 것 테스트
	leftKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered; // 딱 한번만 누름
	leftKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Negative);
	spMoveEditorInputAction->AddInputMappingInfo(leftKeyInputMappingInfo);

	InputActionMappingInfo rightKeyInputMappingInfo;
	rightKeyInputMappingInfo.inputValue = EInputValue::Right;
	rightKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered;
	spMoveEditorInputAction->AddInputMappingInfo(rightKeyInputMappingInfo);

	InputActionMappingInfo upKeyInputMappingInfo;
	upKeyInputMappingInfo.inputValue = EInputValue::Up; // 왼쪽키를 누르면 작동하는 것 테스트
	upKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered; // 딱 한번만 누름
	upKeyInputMappingInfo.inputActionValueModifierBitset.BitsOn(EInputActionValueModifierType::Swizzle, EInputActionValueModifierType::Negative);
	spMoveEditorInputAction->AddInputMappingInfo(upKeyInputMappingInfo);

	InputActionMappingInfo downKeyInputMappingInfo;
	downKeyInputMappingInfo.inputValue = EInputValue::Down;
	downKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered;
	downKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Swizzle);
	spMoveEditorInputAction->AddInputMappingInfo(downKeyInputMappingInfo);

	LocalPlayerInputSystem::I()->BindInputActionHandler(spMoveEditorInputAction, this, &EditorActor::OnDirectionKeyHandler);
#pragma endregion

	spInputMappingContext->AddInputAction(spMoveEditorInputAction);
#pragma endregion
#pragma endregion

	ApplyScreenCenterPosition();

	EditorActorMoveComponent* pEditorActorMoveComponent = AddComponent<EditorActorMoveComponent>();
	pEditorActorMoveComponent->SetMoveSpeed(200.0f);
	pEditorActorMoveComponent->CalculateMovableRangeRect();
}

bool EditorActor::Update(float deltaSeconds)
{
	return Super::Update(deltaSeconds);
}

void EditorActor::Cleanup()
{
	Super::Cleanup();
	LocalPlayerInputSystem::I()->RemoveInputMappingContext(g_editorActorInputMappingContextId);
}

void EditorActor::OnMouseLeftButtonHandler(const InputActionValue* pInputAction)
{
	UNREFERENCED_PARAMETER(pInputAction);

	// 현재 마우스 좌표를 타일에 적용
	m_spWorldTileMapActor->ApplyCurrentMousePositionToTile();
	DEFAULT_TRACE_LOG("마우스 왼쪽 버튼");
}

void EditorActor::OnMouseRightButtonHandler(const InputActionValue* pInputAction)
{
	UNREFERENCED_PARAMETER(pInputAction);

	SceneManager::I()->ReserveNextScene<GameScene>();
	DEFAULT_TRACE_LOG("마우스 오른쪽 버튼");
}

void EditorActor::OnDirectionKeyHandler(const InputActionValue* pInputAction)
{
	EditorActorMoveComponent* pEditorActorMoveComponent = FindComponent<EditorActorMoveComponent>();
	pEditorActorMoveComponent->SetMoveDirection(pInputAction->BringValue<Vector2d>());

#if 0
	const Vector2d& vMoveDir = pInputAction->BringValue<Vector2d>();
	DEFAULT_TRACE_LOG("(%f %f)", vMoveDir.x, vMoveDir.y);
#endif
}