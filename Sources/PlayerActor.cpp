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
#include "Flipbook.h"
#include "Camera.h"
#include "PlayerState.h"

PlayerActor::~PlayerActor()
{
	Cleanup();
}

void PlayerActor::Startup()
{
	Super::Startup();

	m_speed = 300.0f;

	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(200, 300);
	spTransformComponent->SetSize(200, 200);

	// �ݶ��̴� ����
	BoxColliderPtr spBoxCollider = std::make_shared<BoxCollider>();
	Size boxSize = Size(100, 100); // �ݶ��̴� ũ��� ���� ���� ����
	spBoxCollider->SetExtents(Size(boxSize.width / 2, boxSize.height / 2));
	
#pragma region �ø��� ���
	CollisionComponentPtr spCollisionComponent = ADD_COMPONENT(this, CollisionComponent);
	spCollisionComponent->ApplyCollider(spBoxCollider);
	spCollisionComponent->SetCollisionObjectType(ECollisionObjectType::Pawn);
	spCollisionComponent->AddCollisionResponse(ECollisionObjectType::Pawn, ECollisionResponseState::Block);

	CollisionComponent::CollisionEnableOption& refCollisionEnableOption = spCollisionComponent->GetCollsionEnableOption();
	refCollisionEnableOption.AddBit(ECollisionEnableOption::Physics);

	auto collisionHitCallback = std::bind(&PlayerActor::OnCollisionHit, this, std::placeholders::_1);
	spCollisionComponent->SetHitCallback(collisionHitCallback);
#pragma endregion

#pragma region �÷��̾� �ø���
	// Idle
	FlipbookPtr spPlayerDownIdleFlipbook = GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerLeftIdle");
	GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerRightIdle");
	GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerUpIdle");
	GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerDownIdle");
	
	// Walk
	GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerLeftWalk");
	GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerRightWalk");
	GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerUpWalk");
	GET_SYSTEM(ResourceSystem)->LoadFlipbook("Texture/Player/PlayerDownWalk");

	FlipbookComponentPtr spFlipbookComponent = ADD_COMPONENT(this, FlipbookComponent);
	spFlipbookComponent->SetFlipbook(spPlayerDownIdleFlipbook);
	spFlipbookComponent->SetLoop(true);
#pragma endregion

	// �÷��̾��� ī�޶� ���� ī�޶�
	CameraComponentPtr spCameraComponent = ADD_COMPONENT(this, CameraComponent);
	GameApplication::I()->SetCurrentCamera(spCameraComponent->GetCamera());

#pragma region �÷��̾� ����
	m_playerStates.resize(PlayerStateEndId::s_id);
	m_playerStates[PlayerIdleStateId::s_id] = std::make_shared<PlayerIdleState>(this);
	m_playerStates[PlayerWalkStateId::s_id] = std::make_shared<PlayerWalkState>(this);
#pragma endregion
}

bool PlayerActor::Update()
{
	Super::Update();

	UpdateInput();
	m_playerStates[m_currentStateIdx]->Update();

	return true;
}

void PlayerActor::Cleanup()
{
	Super::Cleanup();

	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();
	spCamera->SetTarget(nullptr);
}

void PlayerActor::UpdateInput()
{
	// �׻� Idle ���·� ����
	m_currentStateIdx = PlayerIdleStateId::s_id;

	// �����Ӵ� �̵������� ���
	float deltaSeconds = GET_SYSTEM(FrameSystem)->GetDeltaSeconds();
	float deltaMove = m_speed * deltaSeconds;

	Point2d deltaPos;
	if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Left))
	{
		deltaPos.x = static_cast<int32>(deltaMove * -1.0f);
		m_currentDir = EPlayerDirection::Left;
		m_currentStateIdx = PlayerWalkStateId::s_id;
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Right))
	{
		deltaPos.x = static_cast<int32>(deltaMove);
		m_currentDir = EPlayerDirection::Right;
		m_currentStateIdx = PlayerWalkStateId::s_id;
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Up))
	{
		deltaPos.y = static_cast<int32>(deltaMove * -1.0f);
		m_currentDir = EPlayerDirection::Up;
		m_currentStateIdx = PlayerWalkStateId::s_id;
	}
	else if (GET_SYSTEM(InputSystem)->IsKeyPress(EInputValue::Down))
	{
		deltaPos.y = static_cast<int32>(deltaMove);
		m_currentDir = EPlayerDirection::Down;
		m_currentStateIdx = PlayerWalkStateId::s_id;
	}

	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->AddPosition(deltaPos);
}

void PlayerActor::OnCollisionHit(CollisionComponentPtr spTargetCollisionComponent)
{
	BoxColliderPtr spBoxCollider = std::dynamic_pointer_cast<BoxCollider>(GET_COMPONENT(this, CollisionComponent)->GetCollider());
	const RECT& intersectedRect = GET_SYSTEM(CollisionSystem)->GetIntersectedRect(spBoxCollider->GetInstersectedRectIndex());

	// �ϴ��� right ����
	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	Point2d pos = spTransformComponent->GetPosition();

	int32 intersectedLength = 0;
	switch (m_currentDir)
	{
	case EPlayerDirection::Left:		 
		intersectedLength = (intersectedRect.right - intersectedRect.left);
		pos.x += intersectedLength;
		break;

	case EPlayerDirection::Right:
		intersectedLength = (intersectedRect.right - intersectedRect.left);
		pos.x -= intersectedLength;
		break;

	case EPlayerDirection::Up:
		intersectedLength = (intersectedRect.bottom - intersectedRect.top);
		pos.y += intersectedLength;
		break;

	case EPlayerDirection::Down:
		intersectedLength = (intersectedRect.bottom - intersectedRect.top);
		pos.y -= intersectedLength;
		break;
	}

	spTransformComponent->SetPosition(pos);

	// �ݶ��̴� ������Ʈ
	spBoxCollider->Update();

	std::string strPos = "Player hit " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " intersectedLength " + std::to_string(intersectedLength);
	strPos += '\n';
	::OutputDebugString(strPos.c_str());
}

void PlayerActor::OnCollisionOverlap(CollisionComponentPtr spTargetCollisionComponent)
{
	::OutputDebugString("player overlap\n");
}