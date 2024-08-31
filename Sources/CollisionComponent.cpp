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
	m_collisionResponseFlag |= (TO_NUM(collisionResponseState) << leftShiftCount); // 왼쪽으로 시프트
}

ECollisionResponseState CollisionComponent::TestCollisionResponseBit(ECollisionObjectType collisionObjectType) const
{
	uint32 leftShiftCount = (TO_NUM(collisionObjectType) + 1) * 2;
	uint32 rightShiftCount = TO_NUM(collisionObjectType) * 2;

	uint32 collisionResponseBit = 0;

	// 변수가 시프트 연산될 때 설정한 바이트 수를 넘어가면 무시되는듯?
	if (collisionObjectType != ECollisionObjectType::LastObjectType)
	{
		collisionResponseBit = (0xFFFFFFFF << leftShiftCount); // 왼쪽으로 시프트
	}

	collisionResponseBit = ~collisionResponseBit; // 비트 반전
	collisionResponseBit = (m_collisionResponseFlag & collisionResponseBit); // 설정한 값과 And 연산
	collisionResponseBit = (collisionResponseBit >> rightShiftCount); // 오른쪽으로 시프트

	// collisionResponseBit는 00 01 10 이렇게 3개가 존재
	return TO_ENUM(collisionResponseBit, ECollisionResponseState);
}