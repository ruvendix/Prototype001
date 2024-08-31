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
#include "TileMap.h"

PlayerActor::~PlayerActor()
{
	Cleanup();
}

void PlayerActor::Startup()
{
	Super::Startup();

	m_speed = 300.0f;

	// 셀 좌표 설정
	m_currentCellPos = Int2d(3, 4);

	// 설정한 셀 좌표를 화면 좌표로 변환
	TileMapPtr spTileMap = GET_SYSTEM(ResourceSystem)->GetTileMap();
	Vec2d playerWorldPos = spTileMap->ConvertCellPositionToWorldPosition(m_currentCellPos);

	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(playerWorldPos);
	spTransformComponent->SetSize(200, 200); // 이건 고정

	// 콜라이더 설정
	BoxColliderPtr spBoxCollider = std::make_shared<BoxCollider>();
	spBoxCollider->SetExtents(spTileMap->GetTileSize() / 3); // 콜라이더 크기는 따로 설정 가능(타일 크기와 맞춤)
	
#pragma region 컬리전 등록
	CollisionComponentPtr spCollisionComponent = ADD_COMPONENT(this, CollisionComponent);
	spCollisionComponent->ApplyCollider(spBoxCollider);
	spCollisionComponent->SetCollisionObjectType(ECollisionObjectType::Pawn);
	spCollisionComponent->AddCollisionResponse(ECollisionObjectType::WorldStatic, ECollisionResponseState::Block);
	spCollisionComponent->AddCollisionResponse(ECollisionObjectType::Pawn, ECollisionResponseState::Block);

	CollisionComponent::CollisionEnableOption& refCollisionEnableOption = spCollisionComponent->GetCollsionEnableOption();
	refCollisionEnableOption.AddBit(ECollisionEnableOption::Physics);

	auto collisionHitCallback = std::bind(&PlayerActor::OnCollisionHit, this, std::placeholders::_1);
	spCollisionComponent->SetHitCallback(collisionHitCallback);
#pragma endregion

#pragma region 플레이어 플립북
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

	// 플레이어의 카메라가 현재 카메라
	CameraComponentPtr spCameraComponent = ADD_COMPONENT(this, CameraComponent);
	GameApplication::I()->SetCurrentCamera(spCameraComponent->GetCamera());

#pragma region 플레이어 상태
	m_playerStates.resize(PlayerStateEndId::s_id);
	m_playerStates[PlayerIdleStateId::s_id] = std::make_shared<PlayerIdleState>(this);
	m_playerStates[PlayerWalkStateId::s_id] = std::make_shared<PlayerWalkState>(this);
#pragma endregion
}

bool PlayerActor::Update()
{
	Super::Update();
	m_playerStates[m_currentStateIdx]->Update();

	return true;
}

void PlayerActor::Cleanup()
{
	Super::Cleanup();

	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();
	spCamera->SetTarget(nullptr);
}

void PlayerActor::ApplyNextCellPosition(const Int2d& nextCellPos, bool bDelay)
{
	if (bDelay == false)
	{
		m_currentCellPos = nextCellPos;
		
		TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
		spTransformComponent->SetPosition(ConvertCellPositionToWorldPosition(nextCellPos));
	}

	m_destCellPos = nextCellPos;
}

void PlayerActor::ApplyNextCellPositionToCurrentCellPosition()
{
	ApplyNextCellPosition(m_destCellPos, false);
}

Vec2d PlayerActor::ConvertCellPositionToWorldPosition(const Int2d& cellPos) const
{
	// 설정한 셀 좌표를 화면 좌표로 변환
	TileMapPtr spTileMap = GET_SYSTEM(ResourceSystem)->GetTileMap();
	Vec2d worldPos = spTileMap->ConvertCellPositionToWorldPosition(cellPos);
	return worldPos;
}

Vec2d PlayerActor::CalcDestinationWorldPosition() const
{
	return ConvertCellPositionToWorldPosition(m_destCellPos);
}

void PlayerActor::OnCollisionHit(CollisionComponentPtr spTargetCollisionComponent)
{
	BoxColliderPtr spBoxCollider = std::dynamic_pointer_cast<BoxCollider>(GET_COMPONENT(this, CollisionComponent)->GetCollider());
	const RECT& intersectedRect = GET_SYSTEM(CollisionSystem)->GetIntersectedRect(spBoxCollider->GetInstersectedRectIndex());

	// 일단은 right 적용
	TransformComponentPtr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	Vec2d pos = spTransformComponent->GetPosition();

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

	// 콜라이더 업데이트
	spBoxCollider->UpdateBoxRect();

	std::string strPos = "Player hit " + std::to_string(pos.x) + " " + std::to_string(pos.y)
		+ " intersectedLength " + std::to_string(intersectedLength) + " idx " + std::to_string(spBoxCollider->GetInstersectedRectIndex());
	strPos += '\n';
	::OutputDebugString(strPos.c_str());
}

void PlayerActor::OnCollisionOverlap(CollisionComponentPtr spTargetCollisionComponent)
{
	::OutputDebugString("player overlap\n");
}