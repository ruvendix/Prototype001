#include "Pch.h"
#include "CollisionComponent.h"

#include "Actor.h"
#include "TransformComponent.h"

CollisionComponent::CollisionComponent()
{

}

CollisionComponent::~CollisionComponent()
{

}

void CollisionComponent::Render()
{
	m_spCollider->Render();
}

void CollisionComponent::PostUpdate()
{
	m_spCollider->Update();
}

bool CollisionComponent::TestIntersect(CollisionComponent::Ptr spTargetCollisionComponent)
{
	ColliderBase::Ptr spTargetCollider = spTargetCollisionComponent->GetCollider();
	if (m_spCollider->TestIntersect(spTargetCollider) == false)
	{
		return false;
	}

	return true;
}

void CollisionComponent::ProcessHitEvent(CollisionComponent::Ptr spTargetCollisionComponent)
{
	if (m_hitCallback != nullptr)
	{
		m_hitCallback(spTargetCollisionComponent);
	}
}

void CollisionComponent::ProcessOverlapEvent(CollisionComponent::Ptr spTargetCollisionComponent)
{
	if (m_overlapCallback != nullptr)
	{
		m_overlapCallback(spTargetCollisionComponent);
	}
}

void CollisionComponent::ApplyCollider(ColliderBase::Ptr spColider)
{
	m_spCollider = spColider;
	m_spCollider->SetOwner(GetOwner());
	m_spCollider->Startup();
}

void CollisionComponent::AddCollisionResponse(ECollisionObjectType collisionObjectType, ECollisionResponseState collisionResponseState)
{
	uint32 leftShiftCount = ENUM_TO_NUM(collisionObjectType) * 2;
	m_collisionResponseFlag |= (ENUM_TO_NUM(collisionResponseState) << leftShiftCount); // �������� ����Ʈ
}

ECollisionResponseState CollisionComponent::TestCollisionResponseBit(ECollisionObjectType collisionObjectType) const
{
	uint32 leftShiftCount = (ENUM_TO_NUM(collisionObjectType) + 1) * 2;
	uint32 rightShiftCount = ENUM_TO_NUM(collisionObjectType) * 2;

	uint32 collisionResponseBit = 0;

	// ������ ����Ʈ ����� �� ������ ����Ʈ ���� �Ѿ�� ���õǴµ�?
	if (collisionObjectType != ECollisionObjectType::LastObjectType)
	{
		collisionResponseBit = (0xFFFFFFFF << leftShiftCount); // �������� ����Ʈ
	}

	collisionResponseBit = ~collisionResponseBit; // ��Ʈ ����
	collisionResponseBit = (m_collisionResponseFlag & collisionResponseBit); // ������ ���� And ����
	collisionResponseBit = (collisionResponseBit >> rightShiftCount); // ���������� ����Ʈ

	// collisionResponseBit�� 00 01 10 �̷��� 3���� ����
	return NUM_TO_ENUM(collisionResponseBit, ECollisionResponseState);
}