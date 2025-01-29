// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class ESceneActorMoveDirection
{
	Left = 0,
	Right,
	Down,
	Up,

	Count
};

class SceneActorMoveComponent : public Component
{
	DECLARE_COMPILETIME_ID(SceneActorMoveComponent)

public:
	SceneActorMoveComponent();
	virtual ~SceneActorMoveComponent();

	// 호출 위치 때문에 Update()와 별개로 작동
	void UpdateMovePosition(float deltaSeconds);

	void ApplyMoveDirectionVector(const Vec2d& vMoveDir);
	bool CheckGoalPosition(float deltaSeconds) const;

	void SetDestinationCellPosition(const Position2d& destCellPos) { m_destCellPos = destCellPos; }
	const Position2d& GetDestinationCellPosition() const { return m_destCellPos; }

	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
	const Vec2d& GetDestinationWorldPosition() const { return m_destPos; }

	ESceneActorMoveDirection GetMoveDirection() const { return m_moveDir; }

private:
	float m_moveSpeed = 0.0f;

	Vec2d m_destPos;
	Position2d m_destCellPos;
	ESceneActorMoveDirection m_moveDir = ESceneActorMoveDirection::Down;
};