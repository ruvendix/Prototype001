#include "Pch.h"
#include "CollisionComponent.h"

#include "SystemManager.h"
#include "CollisionSystem.h"
#include "Actor.h"
#include "TransformComponent.h"

const char* CollisionComponent::s_szNullObjectType = "Null";
CollisionResponseInfo CollisionComponent::s_nullCollisionResponseInfo(s_szNullObjectType, ECollisionResponseState::Ignore);

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

void CollisionComponent::AddCollisionResponseInfo(const std::string& strObjectTypeTag, ECollisionResponseState collisionResponseState)
{
	CollisionResponseInfo newCollisionResponseInfo = GET_SYSTEM(CollisionSystem)->CreateCollisionResponseInfo(strObjectTypeTag);
	if (newCollisionResponseInfo.strObjectTypeTag == s_szNullObjectType)
	{
		return;
	}

	// 태그는 적용되었으니 상태만 설정
	newCollisionResponseInfo.collisionResponseState = collisionResponseState;
	m_collisionResponseInfos.emplace_back(newCollisionResponseInfo);
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

const CollisionResponseInfo& CollisionComponent::FindCollisionResponseInfo(const std::string& strObjectTypeTag)
{
	const auto& foundIter = std::find_if(m_collisionResponseInfos.begin(), m_collisionResponseInfos.end(),
		[strObjectTypeTag](CollisionResponseInfo& elem)
		{
			return (strObjectTypeTag == elem.strObjectTypeTag);
		});

	if (foundIter == m_collisionResponseInfos.cend())
	{
		return s_nullCollisionResponseInfo;
	}

	return (*foundIter);
}