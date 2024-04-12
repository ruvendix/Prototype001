#pragma once

#include "PawnActor.h"

class MonsterActor : public PawnActor
{
public:
	using PawnActor::PawnActor;
	using Super = PawnActor;

public:
	virtual ~MonsterActor();

	virtual void Startup() override;

	void OnCollisionHit(CollisionComponentPtr spTargetCollisionComponent);
	void OnCollisionOverlap(CollisionComponentPtr spTargetCollisionComponent);
};