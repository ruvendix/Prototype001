#include "Pch.h"
#include "PlayerActor.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "FrameSystem.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "FlipbookComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "BoxCollider.h"

PlayerActor::~PlayerActor()
{
	Cleanup();
}

void PlayerActor::Startup()
{
	PawnActor::Startup();

	m_speed = 300.0f;

	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(200, 300);
	spTransformComponent->SetSize(200, 200);

	// 콜라이더 설정
	BoxCollider::Ptr spBoxCollider = std::make_shared<BoxCollider>();
	spBoxCollider->SetPosition(spTransformComponent->GetPosition());
	Size boxSize = Size(100, 100); // 콜라이더 크기는 따로 설정 가능
	spBoxCollider->SetExtents(Size(boxSize.width / 2, boxSize.height / 2));

	ColliderComponent::Ptr spColliderComponent = ADD_COMPONENT(this, ColliderComponent);
	spColliderComponent->SetCollider(spBoxCollider);

	// 플립북 기본 데이터 설정
	FlipbookData flipbookData;
	flipbookData.excludeColorKey = RGB(128, 128, 128);
	flipbookData.tileCount = int2d(24, 12);

#pragma region 플레이어 플립북
	// Idle
	flipbookData.tileStartPos = Point2d(0, 0);
	flipbookData.keyframeCount = 10;
	flipbookData.keyframeInterval = 0.1f;

	Flipbook::Ptr spPlayerLeftIdleFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerLeftIdle");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerLeft.bmp");
	spPlayerLeftIdleFlipbook->CreateKeyframes("PlayerLeftIdle", flipbookData);

	Flipbook::Ptr spPlayerRightIdleFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerRightIdle");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerRight.bmp");
	spPlayerRightIdleFlipbook->CreateKeyframes("PlayerRightIdle", flipbookData);

	Flipbook::Ptr spPlayerUpIdleFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerUpIdle");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerUp.bmp");
	spPlayerUpIdleFlipbook->CreateKeyframes("PlayerUpIdle", flipbookData);

	Flipbook::Ptr spPlayerDownIdleFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerDownIdle");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerDown.bmp");
	spPlayerDownIdleFlipbook->CreateKeyframes("PlayerDownIdle", flipbookData);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Walk
	flipbookData.tileStartPos = Point2d(0, 1);
	flipbookData.keyframeInterval = 0.05f;

	Flipbook::Ptr spPlayerLeftWalkFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerLeftWalk");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerLeft.bmp");
	spPlayerLeftWalkFlipbook->CreateKeyframes("PlayerLeftWalk", flipbookData);

	Flipbook::Ptr spPlayerRightWalkFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerRightWalk");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerRight.bmp");
	spPlayerRightWalkFlipbook->CreateKeyframes("PlayerRightWalk", flipbookData);

	Flipbook::Ptr spPlayerUpWalkFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerUpWalk");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerUp.bmp");
	spPlayerUpWalkFlipbook->CreateKeyframes("PlayerUpWalk", flipbookData);

	Flipbook::Ptr spPlayerDownWalkFlipbook = GET_SYSTEM(ResourceSystem)->CreateFlipbook("PlayerDownWalk");
	flipbookData.spTex = GET_SYSTEM(ResourceSystem)->LoadTexture("Sprite/Player/PlayerDown.bmp");
	spPlayerDownWalkFlipbook->CreateKeyframes("PlayerDownWalk", flipbookData);
#pragma endregion

	FlipbookComponent::Ptr spFlipbookComponent = ADD_COMPONENT(this, FlipbookComponent);
	spFlipbookComponent->SetFlipbook(spPlayerDownIdleFlipbook);
	spFlipbookComponent->SetLoop(true);

	// 플레이어의 카메라가 현재 카메라
	CameraComponent::Ptr spCameraComponent = ADD_COMPONENT(this, CameraComponent);
	GameApplication::I()->SetCurrentCamera(spCameraComponent->GetCamera());
}

bool PlayerActor::Update()
{
	PawnActor::Update();

	std::array<std::string, ENUM_TO_NUM(EPlayerDirection::Count)> idleStrings =
	{
		"PlayerLeftIdle",
		"PlayerRightIdle",
		"PlayerUpIdle",
		"PlayerDownIdle"
	};

	std::string strCurrentFlipbook = idleStrings[ENUM_TO_NUM(m_currentDir)];
	FlipbookComponent::Ptr spFlipbookComponent = GET_COMPONENT(this, FlipbookComponent);

	// 프레임당 이동량으로 계산
	float deltaSeconds = GET_SYSTEM(FrameSystem)->GetDeltaSeconds();
	float deltaMove = m_speed * deltaSeconds;

	Point2d deltaPos;
	if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Left))
	{
		deltaPos.x = static_cast<int32>(deltaMove * -1.0f);
		strCurrentFlipbook = "PlayerLeftWalk";
		m_currentDir = EPlayerDirection::Left;
	}

	if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Right))
	{
		deltaPos.x = static_cast<int32>(deltaMove);
		strCurrentFlipbook = "PlayerRightWalk";
		m_currentDir = EPlayerDirection::Right;
	}

	if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Up))
	{
		deltaPos.y = static_cast<int32>(deltaMove * -1.0f);
		strCurrentFlipbook = "PlayerUpWalk";
		m_currentDir = EPlayerDirection::Up;
	}

	if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Down))
	{
		deltaPos.y = static_cast<int32>(deltaMove);
		strCurrentFlipbook = "PlayerDownWalk";
		m_currentDir = EPlayerDirection::Down;
	}

	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->AddPosition(deltaPos);

	Flipbook::Ptr spPlayerFlipbook = GET_SYSTEM(ResourceSystem)->FindFlipbook(strCurrentFlipbook.c_str());
	spFlipbookComponent->SetFlipbook(spPlayerFlipbook);

	// 콜라이더 업데이트
	ColliderComponent::Ptr spColliderComponent = GET_COMPONENT(this, ColliderComponent);
	BoxCollider::Ptr spBoxCollider = std::dynamic_pointer_cast<BoxCollider>(spColliderComponent->GetCollider());
	spBoxCollider->SetPosition(spTransformComponent->GetPosition());

	return true;
}

void PlayerActor::Render()
{
	PawnActor::Render();
}

void PlayerActor::Cleanup()
{
	PawnActor::Cleanup();

	Camera::Ptr spCamera = GameApplication::I()->GetCurrentCamera();
	spCamera->SetTarget(nullptr);
}