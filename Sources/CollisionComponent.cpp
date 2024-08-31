#include "Pch.h"
#include "CollisionComponent.h"

#include "ColliderBase.h"

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

bool CollisionComponent::PostUpdate()
{
	return (m_spCollider->PostUpdate());
}

bool CollisionComponent::TestIntersect(CollisionComponentPtr spTargetCollisionComponent)
{
	ColliderBasePtr spTargetCollider = spTargetCollisionComponent->GetCollider();
	if (m_spCollider->TestIntersect(spTargetCollider) == false)
	{
		return false;
	}

	return true;
}

void CollisionComponent::ProcessHitEvent(CollisionComponentPtr spTargetCollisionComponent)
{
	if (m_hitCallback != nullptr)
	{
		m_hitCallback(spTargetCollisionComponent);
	}
}

void CollisionComponent::ProcessOverlapEvent(CollisionComponentPtr spTargetCollisionComponent)
{
	if (m_overlapCallback != nullptr)
	{
		m_overlapCallback(spTargetCollisionComponent);
	}
}

void CollisionComponent::ApplyCollider(ColliderBasePtr spColider)
{
	m_spCollider = spColider;
	m_spCollider->SetActor(GetOwner());
	m_spCollider->Startup();
}

void CollisionComponent::AddCollisionResponse(ECollisionObjectType collisionObjectType, ECollisionResponseState collisionResponseState)
{
	uint32 leftShiftCount = TO_NUM(collisionObjectType) * 2;
	m_collisionResponseFlag |= (TO_NUM(collisionResponseState) << leftShiftCount); // �������� ����Ʈ
}

ECollisionResponseState CollisionComponent::TestCollisionResponseBit(ECollisionObjectType collisionObjectType) const
{
	uint32 leftShiftCount = (TO_NUM(collisionObjectType) + 1) * 2;
	uint32 rightShiftCount = TO_NUM(collisionObjectType) * 2;

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
	return TO_ENUM(collisionResponseBit, ECollisionResponseState);
}