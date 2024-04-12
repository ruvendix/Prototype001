#pragma once

#include "PawnActor.h"

enum class EPlayerDirection
{
	Left = 0,
	Right,
	Up,
	Down,
	Count,
};

class PlayerActor : public PawnActor
{
public:
	using PawnActor::PawnActor;

	using Super = PawnActor;

public:
	virtual ~PlayerActor();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Cleanup() override;

	void OnCollisionHit(CollisionComponentPtr spTargetCollisionComponent);
	void OnCollisionOverlap(CollisionComponentPtr spTargetCollisionComponent);

private:
	float m_speed = 0.0f;
	EPlayerDirection m_currentDir = EPlayerDirection::Down;
};