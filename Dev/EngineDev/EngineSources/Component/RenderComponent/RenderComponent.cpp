// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "RenderComponent.h"

DEFINE_COMPILETIME_ID(RenderComponent, ComponentIdCounter)

RenderComponent::RenderComponent()
{
	SetRenderComponent(true);
}

RenderComponent::~RenderComponent()
{

}

Position2d RenderComponent::CalculateRenderingStartPosition()
{
	// ��ǥ ��������
	const TransformComponent* pTransformComponent = GetOwner()->GetComponent<TransformComponent>();
	ASSERT_LOG(pTransformComponent != nullptr);

	// ���� ��ǥ�� �Ǻ�
	Vector2d renderingPos = SceneRenderer::I()->ConvertWorldPositionToRenderingPosition(pTransformComponent->GetPosition());

	Position2d renderingStartPos;
	renderingStartPos.x = static_cast<int32>(renderingPos.x) - (pTransformComponent->GetWidth() / 2);
	renderingStartPos.y = static_cast<int32>(renderingPos.y) - (pTransformComponent->GetHeight() / 2);
	//DEFAULT_TRACE_LOG("������ ���� ��ǥ(%d, %d)", renderingStartPos.x, renderingStartPos.y);

	return renderingStartPos;
}