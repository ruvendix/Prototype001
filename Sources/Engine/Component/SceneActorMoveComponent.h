// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class ESceneActorLookAtType
{
	Left = 0,
	Right,
	Down,
	Up,

	Count
};

class SceneActorMoveComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	SceneActorMoveComponent();
	virtual ~SceneActorMoveComponent();

public:
	virtual bool Update(float deltaSeconds) override;

public:
	void ApplyMoveDirectionVector(const Vec2d& vMoveDir);
	bool CheckGoalPosition(float deltaSeconds) const;

	void ResetMoveDirection();

	void SetDestinationCellPosition(const Position2d& destCellPos) { m_destCellPos = destCellPos; }
	const Position2d& GetDestinationCellPosition() const { return m_destCellPos; }

	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
	const Vec2d& GetDestinationWorldPosition() const { return m_destPos; }

	ESceneActorLookAtType GetLookAtType() const { return m_lookAtType; }

private:
	float m_moveSpeed = 0.0f;
	Vec2d m_vMoveDir;

	Vec2d m_destPos;
	Position2d m_destCellPos;
	ESceneActorLookAtType m_lookAtType = ESceneActorLookAtType::Count;
};