// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class CellActorMoveComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	CellActorMoveComponent();
	virtual ~CellActorMoveComponent();

public:
	virtual bool Update(float deltaSeconds) override;

public:
	void ApplyMoveDirection(const Vec2d& vMoveDir);
	bool CheckGoalPosition(float deltaSeconds) const;

	void ResetMoveDirection();
	void ApplyDestinationDataToOwner();

	void SetDestinationCellPosition(const Position2d& destCellPos) { m_destCellPos = destCellPos; }
	const Position2d& GetDestinationCellPosition() const { return m_destCellPos; }

	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
	const Vec2d& GetDestinationWorldPosition() const { return m_destPos; }

private:
	float m_moveSpeed = 0.0f;
	Vec2d m_vMoveDir;

	Vec2d m_destPos;
	Position2d m_destCellPos;
};