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