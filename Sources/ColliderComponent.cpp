#include "Pch.h"
#include "ColliderComponent.h"

#include "Actor.h"
#include "TransformComponent.h"

ColliderComponent::ColliderComponent()
{

}

ColliderComponent::~ColliderComponent()
{

}

bool ColliderComponent::Update()
{
	return m_spCollider->Update();
}

void ColliderComponent::Render()
{
	m_spCollider->Render();
}

bool ColliderComponent::TestIntersect(ColliderComponent::Ptr spTargetColliderComponent)
{
	ColliderBase::Ptr spTargetCollider = spTargetColliderComponent->GetCollider();
	if (m_spCollider->TestIntersect(spTargetCollider) == false)
	{
		return false;
	}

	return true;
}

ECollisionState ColliderComponent::FindCollisonState(ColliderComponent::Ptr spTargetColliderComponent) const
{
	auto foundIter = m_coliisionStates.find(spTargetColliderComponent);
	if (foundIter == m_coliisionStates.cend())
	{
		return ECollisionState::None;
	}

	return foundIter->second;
}

void ColliderComponent::RegisterCollisionCallback(CollisionCallback collisionCallback, ECollisionState collsionCallbackType)
{
	m_collisionCallbacks[ENUM_TO_NUM(collsionCallbackType)] = collisionCallback;
}

void ColliderComponent::ProcessCollsionCallback(ColliderComponent::Ptr spTargetColliderComponent, ECollisionState collsionCallbackType)
{
	CollisionCallback collisionCallback = m_collisionCallbacks[ENUM_TO_NUM(collsionCallbackType)];
	if (collisionCallback != nullptr)
	{
		collisionCallback(spTargetColliderComponent);
	}
}