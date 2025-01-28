// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "Engine/Actor/WorldTileMapActor.h"
#include "PlayerInputSystem.h"
#include "PlayerState.h"


namespace
{
	std::array<std::string, TO_NUM(ESceneActorMoveDirection::Count)> g_arrPlayerIdleStateDynamicSpriteStringTable =
	{
		"PlayerIdleLeft",
		"PlayerIdleRight"
		"PlayerIdleDown",
		"PlayerIdleUp"
	};

	std::array<std::string, TO_NUM(ESceneActorMoveDirection::Count)> g_arrPlayerWalkStateDynamicSpriteStringTable =
	{
		"PlayerWalkLeft",
		"PlayerWalkRight",
		"PlayerWalkDown",
		"PlayerWalkUp"
	};
}

struct PlayerSpriteStartupInfo
{
	std::string strSpriteName;
	std::string strUseTexturePath;
	int32 spriteLine = 0;
};

class PlayerActor::Pimpl
{
	DEFINE_PIMPL(PlayerActor)

public:
	void LoadAndStartupPlayerSprite();
	void ProcessKeyboardDownImpl(ESceneActorMoveDirection moveDir);
};

void PlayerActor::Pimpl::LoadAndStartupPlayerSprite()
{
	std::array<std::string, TO_NUM(ESceneActorMoveDirection::Count)> arrPlayerTexturePathTable =
	{
		"Assets/Texture/Player/PlayerLeft.bmp",
		"Assets/Texture/Player/PlayerRight.bmp",
		"Assets/Texture/Player/PlayerDown.bmp",
		"Assets/Texture/Player/PlayerUp.bmp",
	};

	// 플레이어 텍스처 로딩
	for (const std::string strPlayerTexturePath : arrPlayerTexturePathTable)
	{
		ResourceMananger::I()->LoadTexture(strPlayerTexturePath);
	}

#pragma region 플레이어 스프라이트 초기화 정보 설정 (일단은 8개)
	std::array<PlayerSpriteStartupInfo, 8> arrPlayerSpriteStartupInfo;
	arrPlayerSpriteStartupInfo[0].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Left)];
	arrPlayerSpriteStartupInfo[0].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Left)];

	arrPlayerSpriteStartupInfo[1].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Right)];
	arrPlayerSpriteStartupInfo[1].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Right)];

	arrPlayerSpriteStartupInfo[2].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Down)];
	arrPlayerSpriteStartupInfo[2].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Down)];

	arrPlayerSpriteStartupInfo[3].strSpriteName = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Up)];
	arrPlayerSpriteStartupInfo[3].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Up)];

	arrPlayerSpriteStartupInfo[4].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Left)];
	arrPlayerSpriteStartupInfo[4].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Left)];
	arrPlayerSpriteStartupInfo[4].spriteLine = 1;

	arrPlayerSpriteStartupInfo[5].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Right)];
	arrPlayerSpriteStartupInfo[5].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Right)];
	arrPlayerSpriteStartupInfo[5].spriteLine = 1;

	arrPlayerSpriteStartupInfo[6].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Down)];
	arrPlayerSpriteStartupInfo[6].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Down)];
	arrPlayerSpriteStartupInfo[6].spriteLine = 1;

	arrPlayerSpriteStartupInfo[7].strSpriteName = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(ESceneActorMoveDirection::Up)];
	arrPlayerSpriteStartupInfo[7].strUseTexturePath = arrPlayerTexturePathTable[TO_NUM(ESceneActorMoveDirection::Up)];
	arrPlayerSpriteStartupInfo[7].spriteLine = 1;
#pragma endregion

	for (const PlayerSpriteStartupInfo& playerSpriteStartupInfo : arrPlayerSpriteStartupInfo)
	{
		// 스프라이트 로딩인데 없으니 생성
		const DynamicSpritePtr& spPlayerIdleDownDynamicSprite = ResourceMananger::I()->CreateDynamicSprite(playerSpriteStartupInfo.strSpriteName);
		ASSERT_LOG(spPlayerIdleDownDynamicSprite != nullptr);
		spPlayerIdleDownDynamicSprite->FindAndSetTexture(playerSpriteStartupInfo.strUseTexturePath);

		for (int32 i = 0; i < 10; ++i)
		{
			spPlayerIdleDownDynamicSprite->AddKeyFrame(i * 200, playerSpriteStartupInfo.spriteLine * 200, 200, 200, 0.1f, RGB(128, 128, 128));
		}
	}

	// 기본 스프라이트 설정 (PlayerIdleDown)
	const DynamicSpritePtr& spDefaultPlayerSprite = ResourceMananger::I()->FindDynamicSprite(arrPlayerSpriteStartupInfo[2].strSpriteName);
	ASSERT_LOG_RETURN(spDefaultPlayerSprite != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spDefaultPlayerSprite);

//#if 0 // StaticSprite
//	// 스프라이트 로딩인데 없으니 생성
//	const StaticSpritePtr& spPlayerIdleDownStaticSprite = ResourceMananger::I()->CreateStaticSprite("PlayerIdleDown");
//	ASSERT_LOG(spPlayerIdleDownStaticSprite != nullptr);
//	spPlayerIdleDownStaticSprite->FindAndSetTexture(strPlayerDownTexPath);
//	spPlayerIdleDownStaticSprite->SetBeginDrawPos(0, 0); // 이거 조절하면서 테스트
//	spPlayerIdleDownStaticSprite->SetDrawSize(200, 200);
//
//	// 스프라이트 설정
//	StaticSpriteComponent* pStaticSpriteComponent = m_pOwner->AddComponent<StaticSpriteComponent>();
//	ASSERT_LOG(pStaticSpriteComponent != nullptr);
//	pStaticSpriteComponent->SetStaticSprite(spPlayerIdleDownStaticSprite);
//#else
//	// 스프라이트 로딩인데 없으니 생성
//	const DynamicSpritePtr& spPlayerIdleDownDynamicSprite = ResourceMananger::I()->CreateDynamicSprite("PlayerIdleDown");
//	ASSERT_LOG(spPlayerIdleDownDynamicSprite != nullptr);
//	spPlayerIdleDownDynamicSprite->SetColorKey(RGB(128, 128, 0));
//	spPlayerIdleDownDynamicSprite->FindAndSetTexture(strPlayerDownTexPath);
//
//	for (int32 i = 0; i < 10; ++i)
//	{
//		spPlayerIdleDownDynamicSprite->AddKeyFrame(i * 200, 0, 200, 200, 0.2f);
//	}
//
//	// 스프라이트 설정
//	DynamicSpriteComponent* pDynamicSpriteComponent = m_pOwner->AddComponent<DynamicSpriteComponent>();
//	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
//	pDynamicSpriteComponent->ApplyDynamicSprite(spPlayerIdleDownDynamicSprite);
//#endif
}

void PlayerActor::Pimpl::ProcessKeyboardDownImpl(ESceneActorMoveDirection moveDir)
{
	const PlayerStatePtr& spPlayerState = m_pOwner->m_spPlayerState;
	if (std::dynamic_pointer_cast<PlayerWalkState>(spPlayerState) != nullptr)
	{
		DEFAULT_TRACE_LOG("이동중!");
		return;
	}

	SceneActorMoveComponent* pMoveComponent = m_pOwner->FindComponent<SceneActorMoveComponent>();
	ASSERT_LOG_RETURN(pMoveComponent != nullptr);

	// 현재 셀 좌표 백업
	Position2d currentCellPos = pMoveComponent->GetDestinationCellPosition();

	// 목표 방향 바꾸고
	pMoveComponent->ApplyMoveDirection(moveDir);

	// 이동 가능한지?
	const std::shared_ptr<WorldTileMapActor>& spWorldTileMapActor = m_pOwner->m_spWorldTileMapActor;
	if (spWorldTileMapActor->CheckMovingAvailableTile(pMoveComponent->GetDestinationCellPosition()) == false)
	{
		// Idle 스프라이트로 바꾸고
		const std::string& strPlayerIdleSprite = g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetMoveDirection())];
		m_pOwner->ChangePlayerSprite(strPlayerIdleSprite);

		pMoveComponent->SetDestinationCellPosition(currentCellPos);
		DEFAULT_TRACE_LOG("이동 못함!");

		return;
	}

	// Walk 스프라이트 바꾸고
	const std::string& strPlayerWalkSprite = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetMoveDirection())];
	m_pOwner->ChangePlayerSprite(strPlayerWalkSprite);

	// 설정한 방향 정보 반영
	m_pOwner->ProcessInput();
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
	Callback leftKeyDownCallback = std::bind(&PlayerActor::OnLeftKeyPressed, this);
	KeyboardInputDevice::I()->BindKeyboardInput(EKeyboardValue::Left, leftKeyDownCallback, nullptr, nullptr);

	Callback rightKeyDownCallback = std::bind(&PlayerActor::OnRightKeyPressed, this);
	KeyboardInputDevice::I()->BindKeyboardInput(EKeyboardValue::Right, rightKeyDownCallback, nullptr, nullptr);

	Callback downKeyDownCallback = std::bind(&PlayerActor::OnDownKeyPressed, this);
	KeyboardInputDevice::I()->BindKeyboardInput(EKeyboardValue::Down, downKeyDownCallback, nullptr, nullptr);

	Callback upKeyDownCallback = std::bind(&PlayerActor::OnUpKeyPressed, this);
	KeyboardInputDevice::I()->BindKeyboardInput(EKeyboardValue::Up, upKeyDownCallback, nullptr, nullptr);

#pragma region 새로운 입력 처리
	// 입력값 저장용
	InputActionValue inputActionValue;

	// 입력값 포함하면서 트리거와 핸들러 저장
	InputActionPtr spInputAction = std::make_unique<InputAction>("Test");
	spInputAction->ApplyInputActionValue(inputActionValue, EInputActionValueType::Vector1);

	// 순서상 여기서 매핑 정보 넣음
	InputActionMappingInfo inputMappingInfo;
	inputMappingInfo.inputValue = EInputValue::Left; // 왼쪽키를 누르면 작동하는 것 테스트
	inputMappingInfo.inputTrigger = EInputTrigger::OnlyPressed; // 딱 한번만 누름
	spInputAction->AddInputMappingInfo(inputMappingInfo);

	// 매핑 컨텍스트 테스트
	InputMappingContextPtr spInputMappingContext = std::make_shared<InputMappingContext>();
	spInputMappingContext->AddInputAction(spInputAction);

	// 입력 시스템이 핵심! 입력 디바이스보다 하위임
	m_spPlayerInputSystem = std::make_shared<PlayerInputSystem>();
	m_spPlayerInputSystem->AddInputMappingContext(spInputMappingContext, 0);

	// IndirectCallFunction 이것마저도 저장인 거임 => 람다만 가능? ㅇㅇ 캡처 기능 때문임
	m_spPlayerInputSystem->BindInputActionHandler(spInputAction, this, &PlayerActor::TestInputHandler, 2.0f, 42.0f);
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

	// 상태 처리
	m_spPlayerState = std::make_shared<PlayerIdleState>(this);

	// 레이어 처리
	SetRenderingLayer(ERenderingLayerType::Player);
}

bool PlayerActor::Update(float deltaSeconds)
{
	// 입력부터 처리
	UpdateInput(deltaSeconds);

	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}
	
	m_playerStateChangeEvent.ExcuteIfBound();
	m_spPlayerState->UpdateState(deltaSeconds);

	return true;
}

void PlayerActor::Cleanup()
{
	return (Super::Cleanup());
}

void PlayerActor::UpdateInput(float deltaSeconds)
{
	m_spPlayerInputSystem->ProcessPlayerInput();
}

void PlayerActor::ProcessInput()
{
	m_spPlayerState->ProcessInput();
}

void PlayerActor::ChangePlayerSprite(const std::string& strNextPlayerSprite)
{
	const DynamicSpritePtr& spNextPlayerSprite = ResourceMananger::I()->FindDynamicSprite(strNextPlayerSprite);
	ASSERT_LOG_RETURN(spNextPlayerSprite != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spNextPlayerSprite);
}

const std::string& PlayerActor::FindPlayerIdleSpriteString(ESceneActorMoveDirection moveDir) const
{
	return (g_arrPlayerIdleStateDynamicSpriteStringTable[TO_NUM(moveDir)]);
}

const std::string& PlayerActor::FindPlayerWalkSpriteString(ESceneActorMoveDirection moveDir) const
{
	return (g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(moveDir)]);
}

void PlayerActor::OnLeftKeyPressed()
{
	DEFAULT_TRACE_LOG("왼쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Left);
}

void PlayerActor::OnRightKeyPressed()
{
	DEFAULT_TRACE_LOG("오른쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Right);
}

void PlayerActor::OnDownKeyPressed()
{
	DEFAULT_TRACE_LOG("아래쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Down);
}

void PlayerActor::OnUpKeyPressed()
{
	DEFAULT_TRACE_LOG("위쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Up);
}

void PlayerActor::OnChangePlayerState(const PlayerStatePtr& spNextPlayerState)
{
	DEFAULT_TRACE_LOG("플레이어 상태 변경!");
	m_spPlayerState = spNextPlayerState;
}

void PlayerActor::TestInputHandler(const InputActionValue* pInputAction, float value1, float value2)
{
	//if (pInputAction->BringValue<bool>() == true)
	//{
	//	DEFAULT_TRACE_LOG("true!");
	//}
	//else
	//{
	//	DEFAULT_TRACE_LOG("false!");
	//}

	DEFAULT_TRACE_LOG("값 (%f)", pInputAction->BringValue<float>());
	DEFAULT_TRACE_LOG("테스트 입력 핸들러!");
}