// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PlayerActor.h"

#include "PlayerImplementation.h"

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

// 이걸 이벤트 리스너에 통합?
CLASS_PIMPL(PlayerActor)
{
	DEFINE_PIMPL(PlayerActor);

public:
	void LoadAndStartupPlayerSprite();
	bool ProcessKeyboardDownImpl(ESceneActorMoveDirection moveDir);
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
		spPlayerIdleDownDynamicSprite->SetColorKey(RGB(128, 128, 128));
		spPlayerIdleDownDynamicSprite->FindAndSetTexture(playerSpriteStartupInfo.strUseTexturePath);

		for (int32 i = 0; i < 10; ++i)
		{
			spPlayerIdleDownDynamicSprite->AddKeyFrame(i * 200, playerSpriteStartupInfo.spriteLine * 200, 200, 200, 0.1f);
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

bool PlayerActor::Pimpl::ProcessKeyboardDownImpl(ESceneActorMoveDirection moveDir)
{
	SceneActorMoveComponent* pMoveComponent = m_pOwner->FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return false;
	}

	// 목표 방향 바꾸고
	pMoveComponent->ApplyMoveDirection(moveDir);

	// 스프라이트 바꾸고
	const std::string& strPlayerSprite = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetMoveDirection())];
	m_pOwner->ChangePlayerSprite(strPlayerSprite);

	// 설정한 방향 정보 반영
	//m_spPlayerImpl->ProcessPlayerInput();

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

	CREATE_PIMPL;

	// 플레이어 스프라이트 로딩 및 초기화
	m_spPimpl->LoadAndStartupPlayerSprite();

#pragma region 입력 처리
	UserInputCallback leftKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Left, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Left, leftKeyDownCallback, nullptr, nullptr);

	UserInputCallback rightKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Right, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Right, rightKeyDownCallback, nullptr, nullptr);

	UserInputCallback downKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Down, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Down, downKeyDownCallback, nullptr, nullptr);

	UserInputCallback upKeyDownCallback = std::bind(&PlayerActor::OnKeyboardDown_Up, this);
	KeyboardInputHandler::I()->BindKeyboardInput(EKeyboardValue::Up, upKeyDownCallback, nullptr, nullptr);
#pragma endregion

#pragma region 플레이어 기본 정보 초기화
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG(pTransformComponent != nullptr);

	// 좌표와 크기 설정
	//pTransformComponent->SetPosition(0.0f, 0.0f);
	pTransformComponent->SetSize(200, 200);

	// 셀 크기를 200으로 가정하면?
	ApplyCellPosition(0, 0);
#pragma endregion

	// 이동 처리
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	pMoveComponent->SetMoveSpeed(200.0f);
	pMoveComponent->SetMoveCellPosition(GetCellPosition()); // 초기화니까 똑같음

	// 상태 처리
	m_spPlayerImpl = std::make_unique<PlayerImplementation>(this);
	m_spPlayerImpl->Startup();
}

bool PlayerActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	m_spPlayerImpl->Update(deltaSeconds);
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

void PlayerActor::ChangePlayerSprite(const std::string& strNextPlayerSprite)
{
	const DynamicSpritePtr& spNextPlayerSprite = ResourceMananger::I()->FindDynamicSprite(strNextPlayerSprite);
	ASSERT_LOG_RETURN(spNextPlayerSprite != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spNextPlayerSprite);
}

void PlayerActor::OnKeyboardDown_Left()
{
	DEFAULT_TRACE_LOG("왼쪽 키");

	// 공통적으로 겹치는 함수들이 있음
	m_spPimpl->ProcessKeyboardDownImpl(ESceneActorMoveDirection::Left);

	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// 목표 방향 바꾸고
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Left);

	// 스프라이트 바꾸고
	const std::string& strPlayerSprite = g_arrPlayerWalkStateDynamicSpriteStringTable[TO_NUM(pMoveComponent->GetMoveDirection())];
	ChangePlayerSprite(strPlayerSprite);

	// 설정한 방향 정보 반영
	m_spPlayerImpl->ProcessPlayerInput();
}

void PlayerActor::OnKeyboardDown_Right()
{
	DEFAULT_TRACE_LOG("오른쪽 키");

	// 스프라이트 바꾸고

	// 목표 방향 설정하고
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// 설정한 방향 정보 반영
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Right);
	m_spPlayerImpl->ProcessPlayerInput();
}

void PlayerActor::OnKeyboardDown_Down()
{
	DEFAULT_TRACE_LOG("아래쪽 키");

	// 스프라이트 바꾸고

	// 목표 방향 설정하고
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// 설정한 방향 정보 반영
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Down);
	m_spPlayerImpl->ProcessPlayerInput();
}

void PlayerActor::OnKeyboardDown_Up()
{
	DEFAULT_TRACE_LOG("위쪽 키");

	// 스프라이트 바꾸고

	// 목표 방향 설정하고
	SceneActorMoveComponent* pMoveComponent = FindComponent<SceneActorMoveComponent>();
	if (pMoveComponent->IsMoving())
	{
		return;
	}

	// 설정한 방향 정보 반영
	pMoveComponent->ApplyMoveDirection(ESceneActorMoveDirection::Up);
	m_spPlayerImpl->ProcessPlayerInput();
}