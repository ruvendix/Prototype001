#pragma once

#include "PawnActor.h"
#include "CollisionComponent.h"

class MonsterActor : public PawnActor
{
public:
	DEFINE_SMART_PTR(MonsterActor);
	using PawnActor::PawnActor;

	using Super = PawnActor;

public:
	virtual ~MonsterActor();

	virtual void Startup() override;

	void OnCollisionHit(CollisionComponent::Ptr spTargetCollisionComponent);
	void OnCollisionOverlap(CollisionComponent::Ptr spTargetCollisionComponent);
};