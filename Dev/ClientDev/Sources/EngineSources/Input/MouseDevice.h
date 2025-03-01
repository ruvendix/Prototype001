// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class MouseDevice : public ICoreLoop
{
public:
	virtual bool Update(float deltaSeconds) override;

	const Position2d& GetCurrentMousePosition() const { return m_currentMousePos; }

private:
	// 마우스 좌표 따로 관리
	Position2d m_currentMousePos;
};