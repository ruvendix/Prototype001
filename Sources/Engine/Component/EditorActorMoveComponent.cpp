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
	Vec2d vMovePos = (m_vMoveDir * m_moveSpeed * deltaSeconds);
	//DEFAULT_TRACE_LOG("(%f %f)", vMovePos.x, vMovePos.y);

	TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	Vec2d vNextPos = pTransformComponent->GetPosition();
	vNextPos += vMovePos;

	vNextPos.x = global::Clamp(vNextPos.x, m_movableRangeRect.left, m_movableRangeRect.right);
	vNextPos.y = global::Clamp(vNextPos.y, m_movableRangeRect.top, m_movableRangeRect.bottom);

	pTransformComponent->SetPosition(vNextPos);
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