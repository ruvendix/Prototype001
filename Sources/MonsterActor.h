#pragma once

#include "PawnActor.h"
#include "ColliderComponent.h"

class MonsterActor : public PawnActor
{
public:
	DEFINE_SMART_PTR(MonsterActor);
	using PawnActor::PawnActor;

public:
	virtual ~MonsterActor();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

	void OnCollisionEnter(ColliderComponent::Ptr spTargetColliderComponent);
	void OnCollisionKeep(ColliderComponent::Ptr spTargetColliderComponent);
	void OnCollisionLeave(ColliderComponent::Ptr spTargetColliderComponent);
};