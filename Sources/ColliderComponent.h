#pragma once

#include "IDefault.h"
#include "ComponentBase.h"
#include "ColliderBase.h"

class ColliderComponent : public ComponentBase
{
public:
	DEFINE_SMART_PTR(ColliderComponent);

public:
	ColliderComponent();
	virtual ~ColliderComponent();

	virtual bool Update() override;
	virtual void Render() override;

	bool TestIntersect(ColliderComponent::Ptr spTargetColliderComponent);

	ColliderBase::Ptr GetCollider() const { return m_spCollider; }
	void              SetCollider(ColliderBase::Ptr spColider) { m_spCollider = spColider; }

private:
	ColliderBase::Ptr m_spCollider = nullptr;
};