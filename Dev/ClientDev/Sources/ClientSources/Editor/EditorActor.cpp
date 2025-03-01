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

#pragma region �Է� ó��
	// ���� ���ؽ�Ʈ �߰�
	InputMappingContextPtr spInputMappingContext = std::make_shared<InputMappingContext>();
	g_editorActorInputMappingContextId = spInputMappingContext->GetId();
	LocalPlayerInputSystem::I()->AddInputMappingContext(spInputMappingContext, 0);

#pragma region ���콺 ���� ��ư���� ����
	// �Է°� �����
	InputActionValue editWorldTileInputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spEditWorldTileInputAction = std::make_unique<InputAction>("EditWorldTile");
	spEditWorldTileInputAction->ApplyInputActionValue(editWorldTileInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo leftMouseButtonInputMappingInfo;
	leftMouseButtonInputMappingInfo.inputValue = EInputValue::MouseLeftButton;
	leftMouseButtonInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spEditWorldTileInputAction->AddInputMappingInfo(leftMouseButtonInputMappingInfo);
	LocalPlayerInputSystem::I()->BindInputActionHandler(spEditWorldTileInputAction, this, &EditorActor::OnMouseLeftButtonHandler);

	// �Է� �ý����� �ٽ�! �Է� ����̽����� ������
	spInputMappingContext->AddInputAction(spEditWorldTileInputAction);
#pragma endregion

#pragma region ���콺 ������ ��ư���� ���� ������ ��ȯ
	// �Է°� �����
	InputActionValue reserveGameSceneInputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spReserveGameSceneInputAction = std::make_unique<InputAction>("ReserveGameScene");
	spReserveGameSceneInputAction->ApplyInputActionValue(reserveGameSceneInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo rightMouseButtonInputMappingInfo;
	rightMouseButtonInputMappingInfo.inputValue = EInputValue::MouseRightButton;
	rightMouseButtonInputMappingInfo.inputTrigger = EInputTrigger::OnlyReleased;
	spReserveGameSceneInputAction->AddInputMappingInfo(rightMouseButtonInputMappingInfo);
	LocalPlayerInputSystem::I()->BindInputActionHandler(spReserveGameSceneInputAction, this, &EditorActor::OnMouseRightButtonHandler);

	// �Է� �ý����� �ٽ�! �Է� ����̽����� ������
	spInputMappingContext->AddInputAction(spReserveGameSceneInputAction);
#pragma endregion

#pragma region ����Ű �̵�
	// �� ���� �̵��̴ϱ� Vector2�� �ذ�

	// �Է°� �����
	InputActionValue moveEditorInputActionValue;

	// �Է°� �����ϸ鼭 Ʈ���ſ� �ڵ鷯 ����
	InputActionPtr spMoveEditorInputAction = std::make_unique<InputAction>("MoveEditor");
	spMoveEditorInputAction->ApplyInputActionValue(moveEditorInputActionValue, EInputActionValueType::Vector2);

#pragma region ����Ű�� ���� �� ����
	InputActionMappingInfo leftKeyInputMappingInfo;
	leftKeyInputMappingInfo.inputValue = EInputValue::Left; // ����Ű�� ������ �۵��ϴ� �� �׽�Ʈ
	leftKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered; // �� �ѹ��� ����
	leftKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Negative);
	spMoveEditorInputAction->AddInputMappingInfo(leftKeyInputMappingInfo);

	InputActionMappingInfo rightKeyInputMappingInfo;
	rightKeyInputMappingInfo.inputValue = EInputValue::Right;
	rightKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered;
	spMoveEditorInputAction->AddInputMappingInfo(rightKeyInputMappingInfo);

	InputActionMappingInfo upKeyInputMappingInfo;
	upKeyInputMappingInfo.inputValue = EInputValue::Up; // ����Ű�� ������ �۵��ϴ� �� �׽�Ʈ
	upKeyInputMappingInfo.inputTrigger = EInputTrigger::Triggered; // �� �ѹ��� ����
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

	// ���� ���콺 ��ǥ�� Ÿ�Ͽ� ����
	m_spWorldTileMapActor->ApplyCurrentMousePositionToTile();
	DEFAULT_TRACE_LOG("���콺 ���� ��ư");
}

void EditorActor::OnMouseRightButtonHandler(const InputActionValue* pInputAction)
{
	UNREFERENCED_PARAMETER(pInputAction);

	SceneManager::I()->ReserveNextScene<GameScene>();
	DEFAULT_TRACE_LOG("���콺 ������ ��ư");
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