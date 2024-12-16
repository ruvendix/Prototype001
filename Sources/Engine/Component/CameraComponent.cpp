// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CameraComponent.h"

DEFINE_COMPILETIME_ID(CameraComponent, ComponentCounter)

CameraComponent::CameraComponent()
{

}

CameraComponent::~CameraComponent()
{

}

bool CameraComponent::PostUpdate(float deltaSeconds)
{
	TransformComponent* pTargetTransform = nullptr;
	if (m_pTargetActor != nullptr)
	{
		pTargetTransform = m_pTargetActor->BringTransformComponent();
	}
	else
	{
		pTargetTransform = GetOwner()->BringTransformComponent();
	}

	const Vec2d& targetWorldPos = pTargetTransform->GetPosition();
	const Size& viewerHalfResolution = SceneRenderer::I()->GetViewerHalfResoultion();

	// 카메라가 이동 가능한 영역만 적용 (월드 크기 필요)
	if (targetWorldPos.x < viewerHalfResolution.width)
	{
		m_cameraOffsetPos.x = 0;
	}
	else
	{
		m_cameraOffsetPos.x = (targetWorldPos.x - viewerHalfResolution.width);
	}

	if (targetWorldPos.y < viewerHalfResolution.height)
	{
		m_cameraOffsetPos.y = 0;
	}
	else
	{
		m_cameraOffsetPos.y = (targetWorldPos.y - viewerHalfResolution.height);
	}

	return true;
}