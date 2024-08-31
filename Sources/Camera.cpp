#include "Pch.h"
#include "Camera.h"

#include "GameApplication.h"
#include "Actor.h"
#include "TransformComponent.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

const Vec2d& Camera::CalcScreenCenterOffsetPosition()
{
	Size screenCenterSize = GameApplication::I()->GetHalfResolution();

	if (m_pTarget == nullptr)
	{
		return m_offsetPos;
	}

	TransformComponentPtr spTargetTransformComponent = GET_COMPONENT(m_pTarget, TransformComponent);
	const Vec2d& targetPos = spTargetTransformComponent->GetPosition();

	// 월드 범위 체크
	const Size& worldSize = GameApplication::I()->GetWorldSize();
	if ((targetPos.x < static_cast<int32>(worldSize.width - screenCenterSize.width)) &&
		(targetPos.x > static_cast<int32>(screenCenterSize.width)))
	{
		m_offsetPos.x = targetPos.x - screenCenterSize.width;
	}

	if ((targetPos.y < static_cast<int32>(worldSize.height - screenCenterSize.height)) &&
		(targetPos.y > static_cast<int32>(screenCenterSize.height)))
	{
		m_offsetPos.y = targetPos.y - screenCenterSize.height;
	}

	return m_offsetPos;
}