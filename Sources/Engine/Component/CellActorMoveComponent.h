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
	virtual ComponentPtr CreateClone() override;

public:
	bool ProcessMoveDirection(const Vector2d& vMoveDir, bool bMoveForward);

	void ApplyMoveDirectionToDestination(const Vector2d& vMoveDir);
	bool TryCheckValidateGoalPosition(float deltaSeconds, bool bForceApplyGoalPos) const;

	void ResetMoveDirectionVector();
	void ApplyDestinationDataToOwner();

	void SetDestinationCellPosition(const Position2d& destCellPos) { m_destCellPos = destCellPos; }
	const Position2d& GetDestinationCellPosition() const { return m_destCellPos; }

	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
	const Vector2d& GetDestinationWorldPosition() const { return m_destPos; }

	const Vector2d& GetMoveDirectionVector() { return m_vMoveDir; }
	bool IsZeroMoveDirectionVector() const { return (m_vMoveDir == Vector2d()); }

private:
	float m_moveSpeed = 0.0f;
	Vector2d m_vMoveDir;

	Vector2d m_destPos;
	Position2d m_destCellPos;
};