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

	// ī�޶� �þ� �ȿ� �ִ��� �Ǵ��ϴ� �����? ��������
	const Vec2d& cameraOffsetPos = BringCameraOffsetPosition();
	Vec2d diffCameraOffsetPos = (targetWorldPos - cameraOffsetPos);
	const Size& viewerResolution = SceneRenderer::I()->GetViewerResolution();

	// ī�޶� �����°��� ���̰� �׻� 0���� ũ�� ȭ�� �ȿ� �־����!

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