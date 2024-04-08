#pragma once

#include "IDefault.h"
#include "ComponentBase.h"
#include "ColliderBase.h"
#include "functional"

enum class ECollisionState
{
	None = -1,
	Enter,
	Keep,
	Leave,
	Count,
};

class ColliderComponent : public ComponentBase
{
public:
	DEFINE_SMART_PTR(ColliderComponent);
	using CollisionCallback = std::function<void(ColliderComponent::Ptr)>;

public:
	ColliderComponent();
	virtual ~ColliderComponent();

	virtual bool Update() override;
	virtual void Render() override;

	bool TestIntersect(ColliderComponent::Ptr spTargetColliderComponent);
	ECollisionState FindCollisonState(ColliderComponent::Ptr spTargetColliderComponent) const;

	void RegisterCollisionCallback(CollisionCallback collisionCallback, ECollisionState collsionCallbackType);
	void ProcessCollsionCallback(ColliderComponent::Ptr spTargetColliderComponent, ECollisionState collsionCallbackType);

	ColliderBase::Ptr GetCollider() const { return m_spCollider; }
	void SetCollider(ColliderBase::Ptr spColider) { m_spCollider = spColider; }

	void EraseCollsionState(ColliderComponent::Ptr spTargetColliderComponent) { m_coliisionStates.erase(spTargetColliderComponent); }
	void SetCollisionState(ColliderComponent::Ptr spTargetColliderComponent, ECollisionState collisionState) { m_coliisionStates[spTargetColliderComponent] = collisionState; }

private:
	ColliderBase::Ptr m_spCollider = nullptr;
	std::unordered_map<ColliderComponent::Ptr, ECollisionState> m_coliisionStates;
	std::array<CollisionCallback, ENUM_TO_NUM(ECollisionState::Count)> m_collisionCallbacks;
};