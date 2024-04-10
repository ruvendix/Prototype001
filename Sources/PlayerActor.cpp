#include "Pch.h"
#include "PlayerActor.h"

#include "GameApplication.h"
#include "SystemManager.h"
#include "ResourceSystem.h"
#include "InputSystem.h"
#include "FrameSystem.h"
#include "CollisionSystem.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "FlipbookComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "BoxCollider.h"

PlayerActor::~PlayerActor()
{
	Cleanup();
}

void PlayerActor::Startup()
{
	Super::Startup();

	m_speed = 300.0f;

	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(200, 300);
	spTransformComponent->SetSize(200, 200);

	// 콜라이더 설정
	BoxCollider::Ptr spBoxCollider = std::make_shared<BoxCollider>();
	Size boxSize = Size(100, 100); // 콜라이더 크기는 따로 설정 가능
	spBoxCollider->SetExtents(Size(boxSize.width / 2, boxSize.height / 2));
	
#pragma region 컬리전 등록
	CollisionComponent::Ptr spCollisionComponent = ADD_COMPONENT(this, CollisionComponent);
	spCollisionComponent->ApplyCollider(spBoxCollider);
	spCollisionComponent->SetObjectTypeTag("Pawn");
	spCollisionComponent->AddCollisionResponseInfo("Pawn", ECollisionResponseState::Block);

	CollisionComponent::CollisionEnableOption& refCollisionEnableOption = spCollisionComponent->GetCollsionEnableOption();
	refCollisionEnableOption.AddBit(ECollisionEnableOption::Physics);

	auto collisionHitCallback = std::bind(&PlayerActor::OnCollisionHit, this, std::placeholders::_1);
	spCollisionComponent->SetHitCallback(collisionHitCallback);
#pragma endregion

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
	Super::Update();

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
	else if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Right))
	{
		deltaPos.x = static_cast<int32>(deltaMove);
		strCurrentFlipbook = "PlayerRightWalk";
		m_currentDir = EPlayerDirection::Right;
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Up))
	{
		deltaPos.y = static_cast<int32>(deltaMove * -1.0f);
		strCurrentFlipbook = "PlayerUpWalk";
		m_currentDir = EPlayerDirection::Up;
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Down))
	{
		deltaPos.y = static_cast<int32>(deltaMove);
		strCurrentFlipbook = "PlayerDownWalk";
		m_currentDir = EPlayerDirection::Down;
	}

	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->AddPosition(deltaPos);

	Flipbook::Ptr spPlayerFlipbook = GET_SYSTEM(ResourceSystem)->FindFlipbook(strCurrentFlipbook.c_str());
	spFlipbookComponent->SetFlipbook(spPlayerFlipbook);

	return true;
}

void PlayerActor::Cleanup()
{
	Super::Cleanup();

	Camera::Ptr spCamera = GameApplication::I()->GetCurrentCamera();
	spCamera->SetTarget(nullptr);
}

void PlayerActor::OnCollisionHit(CollisionComponent::Ptr spTargetCollisionComponent)
{
	BoxCollider::Ptr spBoxCollider = std::dynamic_pointer_cast<BoxCollider>(GET_COMPONENT(this, CollisionComponent)->GetCollider());
	const RECT& intersectedRect = GET_SYSTEM(CollisionSystem)->GetIntersectedRect(spBoxCollider->GetInstersectedRectIndex());

	// 일단은 right 적용
	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	Point2d pos = spTransformComponent->GetPosition();

	switch (m_currentDir)
	{
	case EPlayerDirection::Left:
		pos.x += (intersectedRect.right - intersectedRect.left);
		break;

	case EPlayerDirection::Right:
		pos.x -= (intersectedRect.right - intersectedRect.left);
		break;

	case EPlayerDirection::Up:
		pos.y += (intersectedRect.bottom - intersectedRect.top);
		break;

	case EPlayerDirection::Down:
		pos.y -= (intersectedRect.bottom - intersectedRect.top);
		break;
	}

	spTransformComponent->SetPosition(pos);

	// 콜라이더 업데이트
	spBoxCollider->Update();

	std::string strPos = "Player hit " + std::to_string(pos.x) + " " + std::to_string(pos.y);
	strPos += '\n';
	::OutputDebugString(strPos.c_str());
}

void PlayerActor::OnCollisionOverlap(CollisionComponent::Ptr spTargetCollisionComponent)
{
	::OutputDebugString("player overlap\n");
}