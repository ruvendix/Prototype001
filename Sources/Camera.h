#pragma once

class Actor;

class Camera
{
public:
	Camera();
	~Camera();

	const Vec2d& CalcScreenCenterOffsetPosition();

	void SetTarget(Actor* pTarget) { m_pTarget = pTarget; }
	Vec2d GetOffsetPosition() const { return m_offsetPos; }

private:
	Actor* m_pTarget = nullptr;
	Vec2d m_offsetPos;
};