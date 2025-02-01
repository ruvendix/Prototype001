// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class EditorActorMoveComponent : public Component
{
	DECLARE_COMPILETIME_ID(EditorActorMoveComponent)

public:
	EditorActorMoveComponent();
	virtual ~EditorActorMoveComponent();

public:
	virtual bool Update(float deltaSeconds) override;

public:
	void CalculateMovableRangeRect();

	void SetMoveDirection(const Vec2d& vMoveDir) { m_vMoveDir = vMoveDir; }
	void SetMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }

private:
	FloatRect m_movableRangeRect;

	Vec2d m_vMoveDir;
	float m_moveSpeed = 0.0f;
};