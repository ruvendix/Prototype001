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
	bool CheckGoalPosition(float deltaSeconds) const;

	void SetDestinationCellPosition(const CellPosition& destCellPos) { m_destCellPos = destCellPos; }
	const CellPosition& GetDestinationCellPosition() const { return m_destCellPos; }

	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
	const Vec2d& GetDestinationWorldPosition() const { return m_destPos; }

	void SetMoving(bool bMoving) { m_bMoving = bMoving; }
	bool IsMoving() const { return (m_bMoving == true); }

	ESceneActorMoveDirection GetMoveDirection() const { return m_moveDir; }

private:
	bool m_bMoving = false;
	float m_moveSpeed = 0.0f;

	Vec2d m_destPos;
	CellPosition m_destCellPos;
	ESceneActorMoveDirection m_moveDir = ESceneActorMoveDirection::Down;
};