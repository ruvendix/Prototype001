#include "Pch.h"
#include "MonsterActor.h"

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
#include "CollisionComponent.h"
#include "BoxCollider.h"

MonsterActor::~MonsterActor()
{
	Cleanup();
}

void MonsterActor::Startup()
{
	Super::Startup();

	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(500, 400);
	spTransformComponent->SetSize(300, 300);

	// 콜라이더 설정
	BoxCollider::Ptr spBoxCollider = std::make_shared<BoxCollider>();
	const Size& boxSize = spTransformComponent->GetSize();
	spBoxCollider->SetExtents(Size(boxSize.width / 2, boxSize.height / 2));

#pragma region 컬리전 등록
	CollisionComponent::Ptr spCollisionComponent = ADD_COMPONENT(this, CollisionComponent);
	spCollisionComponent->ApplyCollider(spBoxCollider);
	spCollisionComponent->SetObjectTypeTag("Pawn");
	spCollisionComponent->AddCollisionResponseInfo("Pawn", ECollisionResponseState::Block);

	CollisionComponent::CollisionEnableOption& refCollisionEnableOption = spCollisionComponent->GetCollsionEnableOption();
	refCollisionEnableOption.AddBit(ECollisionEnableOption::Physics);

	auto collisionHitCallback = std::bind(&MonsterActor::OnCollisionHit, this, std::placeholders::_1);
	spCollisionComponent->SetHitCallback(collisionHitCallback);
#pragma endregion
}

void MonsterActor::OnCollisionHit(CollisionComponent::Ptr spTargetCollisionComponent)
{
	::OutputDebugString("monster hit\n");
}

void MonsterActor::OnCollisionOverlap(CollisionComponent::Ptr spTargetCollisionComponent)
{
	::OutputDebugString("monster overlap\n");
}