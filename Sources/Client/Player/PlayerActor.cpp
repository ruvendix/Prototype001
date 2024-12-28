// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "PlayerState.h"
#include "Engine/Actor/WorldTileMapActor.h"

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
	SceneActorMoveComponent* pMoveComponent = m_pOwner->FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// 현재 셀 좌표 백업
	CellPosition currentCellPos = pMoveComponent->GetDestinationCellPosition();

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
	m_pOwner->ProcessPlayerInput();
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

	// 이벤트 핸들러 등록
	RegisterEventHandler<PlayerStateChangeEvent>(&PlayerActor::OnChangeState);

	// 플레이어 스프라이트 로딩 및 초기화
	m_spPimpl->LoadAndStartupPlayerSprite();

#pragma region 입력 처리
	Callback leftKeyDownCallback = std::bind(&PlayerActor::OnLeftKeyDown, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Left, leftKeyDownCallback, nullptr, nullptr);

	Callback rightKeyDownCallback = std::bind(&PlayerActor::OnRightKeyDown, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Right, rightKeyDownCallback, nullptr, nullptr);

	Callback downKeyDownCallback = std::bind(&PlayerActor::OnDownKeyDown, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Down, downKeyDownCallback, nullptr, nullptr);

	Callback upKeyDownCallback = std::bind(&PlayerActor::OnUpKeyDown, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Up, upKeyDownCallback, nullptr, nullptr);
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
}

bool PlayerActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	m_spPlayerState->UpdateState(deltaSeconds);
	return true;
}

bool PlayerActor::PostUpdate(float deltaSeconds)
{
	return (Super::PostUpdate(deltaSeconds));
}

void PlayerActor::Cleanup()
{
	return (Super::Cleanup());
}

void PlayerActor::ProcessPlayerInput()
{
	m_spPlayerState->ProcessPlayerInput();
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

void PlayerActor::OnLeftKeyDown()
{
	DEFAULT_TRACE_LOG("왼쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Left);
}

void PlayerActor::OnRightKeyDown()
{
	DEFAULT_TRACE_LOG("오른쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Right);
}

void PlayerActor::OnDownKeyDown()
{
	DEFAULT_TRACE_LOG("아래쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Down);
}

void PlayerActor::OnUpKeyDown()
{
	DEFAULT_TRACE_LOG("위쪽 키");
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Up);
}

void PlayerActor::OnChangeState(const CallbackArgs& eventArgs)
{
	DEFAULT_TRACE_LOG("체인지 스테이트!");
	m_spPlayerState = std::any_cast<PlayerStatePtr>(eventArgs[0]);
}