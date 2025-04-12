// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class EnemyRespawner;

class EnemyMonsterActor : public ActionableActor
{
	DEFINE_ROOT_COMPILETIME_ID_CLASS;
	using Super = ActionableActor;

public:
	using Super::Super;

	EnemyMonsterActor(const EnemyMonsterActor& srcActor);
	virtual ~EnemyMonsterActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	virtual void ProcessMoveDirection(const Vector2d& vMoveDir) override;
	virtual bool CheckMovingState() const override;

public:
	int32 GetChaseRange() const { return m_chaseRange; }

private:
	int32 m_chaseRange = 7;
};