// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CameraActor.h"

CameraActor::~CameraActor()
{

}

void CameraActor::Startup()
{
	Super::Startup();

	SetActorFlagBitOff(EActorFlag::RenderingTarget);
	AddComponent<CameraComponent>();
}

bool CameraActor::CheckInCameraVision(const Actor* pActor) const
{
	const Vector2d& targetWorldPos = pActor->BringPosition();
	return (CheckInCameraVision(targetWorldPos));
}

bool CameraActor::CheckInCameraVision(const Vector2d& worldPos) const
{
	// 카메라 시야 안에 있는지 판단하는 방법은? 오프셋임
	const Vector2d& cameraOffsetPos = BringCameraOffsetPosition();
	Vector2d diffCameraOffsetPos = (worldPos - cameraOffsetPos);
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

const Vector2d& CameraActor::BringCameraOffsetPosition() const
{
	const CameraComponent* pCameraComponent = FindConstComponent<CameraComponent>();
	return pCameraComponent->GetCameraOffsetPosition();
}