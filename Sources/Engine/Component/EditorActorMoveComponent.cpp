// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EditorActorMoveComponent.h"

DEFINE_COMPILETIME_ID(EditorActorMoveComponent, ComponentIdCounter)

EditorActorMoveComponent::EditorActorMoveComponent()
{

}

EditorActorMoveComponent::~EditorActorMoveComponent()
{

}

bool EditorActorMoveComponent::Update(float deltaSeconds)
{
	TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	Vec2d vActorNextPos = pTransformComponent->GetPosition();

	Vec2d vMovePos = (m_vMoveDir * m_moveSpeed * deltaSeconds);
	vActorNextPos += vMovePos;

	vActorNextPos.x = global::Clamp(vActorNextPos.x, m_movableRangeRect.left, m_movableRangeRect.right);
	vActorNextPos.y = global::Clamp(vActorNextPos.y, m_movableRangeRect.top, m_movableRangeRect.bottom);

	pTransformComponent->SetPosition(vActorNextPos);
	return true;
}

void EditorActorMoveComponent::CalculateMovableRangeRect()
{
	const Size& worldSize = WorldContext::I()->GetWorldSize();
	const Size& halfResolutionSize = SceneRenderer::I()->GetViewerHalfResoultion();

	m_movableRangeRect.left = static_cast<float>(halfResolutionSize.width);
	m_movableRangeRect.right = static_cast<float>(worldSize.width - halfResolutionSize.width);
	m_movableRangeRect.top = static_cast<float>(halfResolutionSize.height);
	m_movableRangeRect.bottom = static_cast<float>(worldSize.height - halfResolutionSize.height);
}