// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CameraComponent.h"

DEFINE_COMPILETIME_ID(CameraComponent, ComponentIdCounter)

CameraComponent::CameraComponent()
{

}

CameraComponent::~CameraComponent()
{

}

bool CameraComponent::Update(float deltaSeconds)
{
	Actor* pTargetActor = nullptr;
	if (m_spTargetActor != nullptr)
	{
		pTargetActor = m_spTargetActor.get();
	}
	else
	{
		pTargetActor = GetOwner();
	}

	const Vector2d& vTargetWorldPos = pTargetActor->BringPosition();
	const Size& viewerHalfResolution = SceneRenderer::I()->GetViewerHalfResoultion();

	// 카메라 오프셋의 기준은 화면 정중앙이므로 이 부분을 고려해야함!

	// 화면 정중앙을 넘어가지 않았을 때는 오프셋 없음
	if (vTargetWorldPos.x < viewerHalfResolution.width)
	{
		m_cameraOffsetPos.x = 0;
	}
	else
	{
		// 카메라가 움직일 수 있는 최대 영역에 도달했는지?
		float cameraMoveLimitWidth = static_cast<float>(WorldContext::I()->GetWorldSize().width - viewerHalfResolution.width);
		if (vTargetWorldPos.x > cameraMoveLimitWidth)
		{
			m_cameraOffsetPos.x = (cameraMoveLimitWidth - viewerHalfResolution.width);
		}
		else
		{
			m_cameraOffsetPos.x = (vTargetWorldPos.x - viewerHalfResolution.width);
		}
	}

	if (vTargetWorldPos.y < viewerHalfResolution.height)
	{
		m_cameraOffsetPos.y = 0;
	}
	else
	{
		// 카메라가 움직일 수 있는 최대 영역에 도달했는지?
		float cameraMoveLimitHeight = static_cast<float>(WorldContext::I()->GetWorldSize().height - viewerHalfResolution.height);
		if (vTargetWorldPos.y > cameraMoveLimitHeight)
		{
			m_cameraOffsetPos.y = (cameraMoveLimitHeight - viewerHalfResolution.height);
		}
		else
		{
			m_cameraOffsetPos.y = (vTargetWorldPos.y - viewerHalfResolution.height);
		}
	}

	return true;
}