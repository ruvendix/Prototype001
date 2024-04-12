#pragma once

class Actor;

class Camera
{
public:
	Camera();
	~Camera();

	const Point2d& CalcScreenCenterOffsetPosition();

	void SetTarget(Actor* pTarget) { m_pTarget = pTarget; }
	const Point2d& GetOffsetPosition() const { return m_offsetPos; }

private:
	Actor*  m_pTarget = nullptr;
	Point2d m_offsetPos;
};