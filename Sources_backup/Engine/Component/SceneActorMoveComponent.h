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

	void ApplyMoveDirection(ESceneActorMoveDirection moveDir);
	bool CheckGoalPosition() const;

	void SetMoveCellPosition(const CellPosition& moveCellPos) { m_moveCellPos = moveCellPos; }

	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
	const Vec2d& GetMovePosition() const { return m_movePos; }

	void SetMoving(bool bMoving) { m_bMoving = bMoving; }
	bool IsMoving() const { return (m_bMoving == true); }

	ESceneActorMoveDirection GetMoveDirection() const { return m_moveDir; }

private:
	bool m_bMoving = false;
	float m_moveSpeed = 0.0f;

	ESceneActorMoveDirection m_moveDir = ESceneActorMoveDirection::Down;
	Vec2d m_movePos;
	CellPosition m_moveCellPos;
};