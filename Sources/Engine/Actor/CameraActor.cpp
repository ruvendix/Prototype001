// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CameraActor.h"

CameraActor::~CameraActor()
{

}

void CameraActor::Startup()
{
	AddComponent<CameraComponent>();
}

bool CameraActor::PostUpdate(float deltaSeconds)
{
	if (Super::PostUpdate(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

bool CameraActor::CheckInCameraVision(const Actor* pActor) const
{
	const TransformComponent* pTargetTransformComponent = pActor->BringTransformComponent();
	ASSERT_LOG(pTargetTransformComponent != nullptr);
	const Vec2d& targetWorldPos = pTargetTransformComponent->GetPosition();

	// 카메라 시야 안에 있는지 판단하는 방법은? 오프셋임
	const Vec2d& cameraOffsetPos = BringCameraOffsetPosition();
	Vec2d diffCameraOffsetPos = (targetWorldPos - cameraOffsetPos);
	const Size& viewerResolution = SceneRenderer::I()->GetViewerResolution();

	// 카메라 오프셋과의 차이가 항상 0보다 크고 화면 안에 있어야함!

	if ((diffCameraOffsetPos.x < 0.0f) ||
		(diffCameraOffsetPos.x > static_cast<float>(viewerResolution.width)))
	{
		return false;
	}

	if ((diffCameraOffsetPos.y < 0.0f) ||
		(diffCameraOffsetPos.y > static_cast<float>(viewerResolution.height)))
	{
		return false;
	}

	return true;
}

const Vec2d& CameraActor::BringCameraOffsetPosition() const
{
	const CameraComponent* pCameraComponent = FindComponent<CameraComponent>();
	return pCameraComponent->GetCameraOffsetPosition();
}