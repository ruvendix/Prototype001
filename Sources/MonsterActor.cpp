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
#include "ColliderComponent.h"
#include "BoxCollider.h"

MonsterActor::~MonsterActor()
{
	Cleanup();
}

void MonsterActor::Startup()
{
	PawnActor::Startup();

	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spTransformComponent->SetPosition(500, 400);
	spTransformComponent->SetSize(300, 300);

	// �ݶ��̴� ����
	BoxCollider::Ptr spBoxCollider = std::make_shared<BoxCollider>();
	spBoxCollider->SetPosition(spTransformComponent->GetPosition());
	const Size& boxSize = spTransformComponent->GetSize();
	spBoxCollider->SetExtents(Size(boxSize.width / 2, boxSize.height / 2));

	ColliderComponent::Ptr spColliderComponent = ADD_COMPONENT(this, ColliderComponent);
	spColliderComponent->SetCollider(spBoxCollider);

#pragma region �ø��� �ݹ� ���
	auto collisionEnterCallback = std::bind(&MonsterActor::OnCollisionEnter, this, std::placeholders::_1);
	spColliderComponent->RegisterCollisionCallback(collisionEnterCallback, ECollisionState::Enter);

	auto collisionKeepCallback = std::bind(&MonsterActor::OnCollisionKeep, this, std::placeholders::_1);
	spColliderComponent->RegisterCollisionCallback(collisionKeepCallback, ECollisionState::Keep);

	auto collisionLeaveCallback = std::bind(&MonsterActor::OnCollisionLeave, this, std::placeholders::_1);
	spColliderComponent->RegisterCollisionCallback(collisionLeaveCallback, ECollisionState::Leave);
#pragma endregion
}

bool MonsterActor::Update()
{
	PawnActor::Update();

	// �ݶ��̴� ������Ʈ
	ColliderComponent::Ptr spColliderComponent = GET_COMPONENT(this, ColliderComponent);
	BoxCollider::Ptr spBoxCollider = std::dynamic_pointer_cast<BoxCollider>(spColliderComponent->GetCollider());

	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(this, TransformComponent);
	spBoxCollider->SetPosition(spTransformComponent->GetPosition());

	return true;
}

void MonsterActor::Render()
{
	PawnActor::Render();
}

void MonsterActor::Cleanup()
{
	PawnActor::Cleanup();
}

void MonsterActor::OnCollisionEnter(ColliderComponent::Ptr spTargetColliderComponent)
{
	::OutputDebugString("monster Enter\n");
}

void MonsterActor::OnCollisionKeep(ColliderComponent::Ptr spTargetColliderComponent)
{
}

void MonsterActor::OnCollisionLeave(ColliderComponent::Ptr spTargetColliderComponent)
{
	::OutputDebugString("monster Leave\n");
}