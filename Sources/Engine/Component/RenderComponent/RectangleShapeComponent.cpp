// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "RectangleShapeComponent.h"

DEFINE_COMPILETIME_ID(RectangleShapeComponent, ComponentIdCounter)

RectangleShapeComponent::RectangleShapeComponent()
{
	SetRenderComponent(true);
}

RectangleShapeComponent::~RectangleShapeComponent()
{

}

void RectangleShapeComponent::Render(HDC hBackBufferDc)
{
	// ��ǥ ��������
	const TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);

	Position2d renderingStartPos = CalculateRenderingStartPosition();

	::Rectangle(hBackBufferDc,
		renderingStartPos.x, renderingStartPos.y,
		renderingStartPos.x + pTransformComponent->GetWidth(),
		renderingStartPos.y + pTransformComponent->GetHeight());
}