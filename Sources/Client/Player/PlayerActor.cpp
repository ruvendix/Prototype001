// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "Engine/Actor/AnimationActor/AnimationActorState.h"

class PlayerActor::Pimpl
{
	DEFINE_PIMPL(PlayerActor)

public:
	void LoadAndStartupPlayerSprite();
	bool DirectionKeyHandlerImpl(const Vec2d& vMoveDir);
	bool SpaceBarKeyHandlerImpl();
};

void PlayerActor::Pimpl::LoadAndStartupPlayerSprite()
{
	// 이걸 확실하게 맞추려면 인덱스마다 넣어야함
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerLeft.bmp", EActorLookAtType::Left);
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerRight.bmp", EActorLookAtType::Right);
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerDown.bmp", EActorLookAtType::Down);
	m_pOwner->LoadActorLookAtTexture("Assets/Texture/Player/PlayerUp.bmp", EActorLookAtType::Up);

	// 상태마다 스프라이트를 미리 만들고
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorIdleState>("PlayerIdle", true);
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorWalkState>("PlayerWalk", true);
	m_pOwner->CreateActorStateLookAtDynamicSprites<AnimationActorAttackState>("PlayerAttack", false);

	// 스프라이트마다 세팅
	m_pOwner->AddActorStateKeyFrames<AnimationActorIdleState>(0, 9, 0, Size{ 200, 200 }, RGB(128, 128, 128), 0.1f);
	m_pOwner->AddActorStateKeyFrames<AnimationActorWalkState>(0, 9, 1, Size{ 200, 200 }, RGB(128, 128, 128), 0.1f);
	m_pOwner->AddActorStateKeyFrames<AnimationActorAttackState>(0, 7, 3, Size{ 200, 200 }, RGB(128, 128, 128), 0.06f);

	// 기본 스프라이트 설정 (PlayerIdleDown)
	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);

	const DynamicSpritePtr& spDefaultPlayerDynamicSprite = m_pOwner->FindActorStateLookAtDynamicSprite<AnimationActorIdleState>(EActorLookAtType::Down);
	pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerDynamicSprite);
}

bool PlayerActor::Pimpl::DirectionKeyHandlerImpl(const Vec2d& vMoveDir)
{
	if (m_pOwner->IsSameAnimationActorState<AnimationActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle일 때만 행동 가능!");
		return false;
	}

	SceneActorMoveComponent* pMoveComponent = m_pOwner->FindComponent<SceneActorMoveComponent>();
	ASSERT_LOG_RETURN_VALUE(pMoveComponent != nullptr, false);

	// 현재 셀 좌표 백업
	Position2d currentCellPos = pMoveComponent->GetDestinationCellPosition();

	// 목표 지점 및 방향 바꾸고
	m_pOwner->ApplyMoveDirection(vMoveDir);
	pMoveComponent->ApplyMoveDirection(vMoveDir);

	// 이동 가능한지?
	const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor = m_pOwner->GetWorldTileMapActor();
	if (spWorldTileMapActor->CheckMovingAvailableTile(pMoveComponent->GetDestinationCellPosition()) == false)
	{
		// Idle 스프라이트로 바꿈
		m_pOwner->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		pMoveComponent->SetDestinationCellPosition(currentCellPos);
		pMoveComponent->ResetMoveDirection();
		DEFAULT_TRACE_LOG("이동 못함!");

		return false;
	}

	// Walk 스프라이트로 바꿈
	m_pOwner->ChangeActorStateDynamicSprite<AnimationActorWalkState>();
	return true;
}

bool PlayerActor::Pimpl::SpaceBarKeyHandlerImpl()
{
	if (m_pOwner->IsSameAnimationActorState<AnimationActorIdleState>() == false)
	{
		DEFAULT_TRACE_LOG("Idle일 때만 행동 가능!");
		return false;
	}

	// Attack 스프라이트로 바꿈
	m_pOwner->ChangeActorStateDynamicSprite<AnimationActorAttackState>();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
PlayerActor::~PlayerActor()
{
	Cleanup();
}

void PlayerActor::Startup()
{
	Super::Startup();
	ALLOC_PIMPL;

	// 플레이어 스프라이트 로딩 및 초기화
	m_spPimpl->LoadAndStartupPlayerSprite();

#pragma region 입력 처리
	InputMappingContextPtr spPlayerInputMappingContext = std::make_shared<InputMappingContext>();
	LocalPlayerInputSystem::I()->AddInputMappingContext(spPlayerInputMappingContext, 0);

#pragma region 플레이어 이동 (Vector2로 해결)
	InputActionValue playerWalkInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spPlayerWalkInputAction = std::make_unique<InputAction>("PlayerWalk");
	spPlayerWalkInputAction->ApplyInputActionValue(playerWalkInputActionValue, EInputActionValueType::Vector2);

	// 순서상 여기서 매핑 정보 넣음
#pragma region 방향키에 따른 값 설정
	InputActionMappingInfo leftKeyInputMappingInfo;
	leftKeyInputMappingInfo.inputValue = EInputValue::Left; // 왼쪽키를 누르면 작동하는 것 테스트
	leftKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed; // 딱 한번만 누름
	leftKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Negative);
	spPlayerWalkInputAction->AddInputMappingInfo(leftKeyInputMappingInfo);

	InputActionMappingInfo rightKeyInputMappingInfo;
	rightKeyInputMappingInfo.inputValue = EInputValue::Right;
	rightKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spPlayerWalkInputAction->AddInputMappingInfo(rightKeyInputMappingInfo);

	InputActionMappingInfo upKeyInputMappingInfo;
	upKeyInputMappingInfo.inputValue = EInputValue::Up; // 왼쪽키를 누르면 작동하는 것 테스트
	upKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed; // 딱 한번만 누름
	upKeyInputMappingInfo.inputActionValueModifierBitset.BitsOn(EInputActionValueModifierType::Swizzle, EInputActionValueModifierType::Negative);
	spPlayerWalkInputAction->AddInputMappingInfo(upKeyInputMappingInfo);

	InputActionMappingInfo downKeyInputMappingInfo;
	downKeyInputMappingInfo.inputValue = EInputValue::Down;
	downKeyInputMappingInfo.inputActionValueModifierBitset.BitOn(EInputActionValueModifierType::Swizzle);
	spPlayerWalkInputAction->AddInputMappingInfo(downKeyInputMappingInfo);
#pragma endregion

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerWalkInputAction, this, &PlayerActor::OnDirectionKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerWalkInputAction);
#pragma endregion

#pragma region 플레이어 공격
	InputActionValue playerAttackInputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spPlayerAttackInputAction = std::make_unique<InputAction>("PlayerAttack");
	spPlayerAttackInputAction->ApplyInputActionValue(playerAttackInputActionValue, EInputActionValueType::Boolean);

	InputActionMappingInfo spaceBarKeyInputMappingInfo;
	spaceBarKeyInputMappingInfo.inputValue = EInputValue::SpaceBar;
	spaceBarKeyInputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed;
	spPlayerAttackInputAction->AddInputMappingInfo(spaceBarKeyInputMappingInfo);

	LocalPlayerInputSystem::I()->BindInputActionHandler(spPlayerAttackInputAction, this, &PlayerActor::OnSpaceBarKeyHandler);
	spPlayerInputMappingContext->AddInputAction(spPlayerAttackInputAction);
#pragma endregion
#pragma endregion

#pragma region 플레이어 기본 정보 초기화
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG(pTransformComponent != nullptr);

	// 좌표와 크기 설정
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(200, 200);

	// 셀 크기는 48로 고정!
	ApplyCellPosition(3, 6);
#pragma endregion

	// 이동 처리
	AddComponent<SceneActorMoveComponent>();
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoveSpeed(90.0f);
	pMoveComponent->SetDestinationCellPosition(GetCellPosition()); // 초기화니까 똑같음

	// 레이어 처리
	SetRenderingLayer(ERenderingLayerType::Creature);
}

bool PlayerActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}
	
	return true;
}

void PlayerActor::Cleanup()
{
	return (Super::Cleanup());
}

void PlayerActor::OnDirectionKeyHandler(const InputActionValue* pInputAction)
{
	const Vec2d& vMoveDir = pInputAction->BringValue<Vec2d>();
	if (m_spPimpl->DirectionKeyHandlerImpl(vMoveDir) == false)
	{
		return;
	}

	ImmediatelyChangePlayerState<AnimationActorWalkState>();
	DEFAULT_TRACE_LOG("걷는 상태로 전환!");
}

void PlayerActor::OnSpaceBarKeyHandler(const InputActionValue* pInputAction)
{
	if (m_spPimpl->SpaceBarKeyHandlerImpl() == false)
	{
		return;
	}
	
	ImmediatelyChangePlayerState<AnimationActorAttackState>();
}