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
	TransformComponent* pTargetTransform = nullptr;
	if (m_spTargetActor != nullptr)
	{
		pTargetTransform = m_spTargetActor->BringTransformComponent();
	}
	else
	{
		pTargetTransform = GetOwner()->BringTransformComponent();
	}

	const Vec2d& targetWorldPos = pTargetTransform->GetPosition();
	const Size& viewerHalfResolution = SceneRenderer::I()->GetViewerHalfResoultion();

	// ī�޶� �������� ������ ȭ�� ���߾��̹Ƿ� �� �κ��� ����ؾ���!

	// ȭ�� ���߾��� �Ѿ�� �ʾ��� ���� ������ ����
	if (targetWorldPos.x < viewerHalfResolution.width)
	{
		m_cameraOffsetPos.x = 0;
	}
	else
	{
		// ī�޶� ������ �� �ִ� �ִ� ������ �����ߴ���?
		float cameraMoveLimitWidth = static_cast<float>(WorldContext::I()->GetWorldSize().width - viewerHalfResolution.width);
		if (targetWorldPos.x > cameraMoveLimitWidth)
		{
			m_cameraOffsetPos.x = (cameraMoveLimitWidth - viewerHalfResolution.width);
		}
		else
		{
			m_cameraOffsetPos.x = (targetWorldPos.x - viewerHalfResolution.width);
		}
	}

	if (targetWorldPos.y < viewerHalfResolution.height)
	{
		m_cameraOffsetPos.y = 0;
	}
	else
	{
		// ī�޶� ������ �� �ִ� �ִ� ������ �����ߴ���?
		float cameraMoveLimitHeight = static_cast<float>(WorldContext::I()->GetWorldSize().height - viewerHalfResolution.height);
		if (targetWorldPos.y > cameraMoveLimitHeight)
		{
			m_cameraOffsetPos.y = (cameraMoveLimitHeight - viewerHalfResolution.height);
		}
		else
		{
			m_cameraOffsetPos.y = (targetWorldPos.y - viewerHalfResolution.height);
		}
	}

	return true;
}