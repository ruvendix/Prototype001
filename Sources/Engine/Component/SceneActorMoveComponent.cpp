// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneActorMoveComponent.h"

DEFINE_COMPILETIME_ID(SceneActorMoveComponent, ComponentIdCounter)

SceneActorMoveComponent::SceneActorMoveComponent()
{

}

SceneActorMoveComponent::~SceneActorMoveComponent()
{

}

void SceneActorMoveComponent::UpdateMovePosition(float deltaSeconds)
{
	TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	Vec2d actorPos = pTransformComponent->GetPosition();

	switch (m_moveDir)
	{
	case ESceneActorMoveDirection::Left:
		actorPos.x += (m_moveSpeed * deltaSeconds * -1.0f);
		break;

	case ESceneActorMoveDirection::Right:
		actorPos.x += (m_moveSpeed * deltaSeconds * 1.0f);
		break;

	case ESceneActorMoveDirection::Down:
		actorPos.y += (m_moveSpeed * deltaSeconds * 1.0f);
		break;

	case ESceneActorMoveDirection::Up:
		actorPos.y += (m_moveSpeed * deltaSeconds * -1.0f);
		break;
	}

	pTransformComponent->SetPosition(actorPos);
}

void SceneActorMoveComponent::ApplyMoveDirection(ESceneActorMoveDirection moveDir)
{
	m_moveDir = moveDir;

	switch (moveDir)
	{
	case ESceneActorMoveDirection::Left:
		m_destCellPos.x -= 1;
		break;

	case ESceneActorMoveDirection::Right:
		m_destCellPos.x += 1;
		break;

	case ESceneActorMoveDirection::Down:
		m_destCellPos.y += 1;
		break;

	case ESceneActorMoveDirection::Up:
		m_destCellPos.y -= 1;
		break;
	}

	// 목표 좌표 계산하기
	CellActor* pSceneActor = GetOwner<CellActor>();
	m_destPos = pSceneActor->ConvertCellPositionToWorldPosition(m_destCellPos);
}

bool SceneActorMoveComponent::CheckGoalPosition(float deltaSeconds) const
{
	TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	const Vec2d& currentPos = pTransformComponent->GetPosition();

	// 이제 그 둘의 거리 비교
	const Vec2d& diffPos = (m_destPos - currentPos);
	float diffLengthSquare = diffPos.CalculateLengthSquare();
	if (diffLengthSquare > (m_moveSpeed * deltaSeconds)) // 숫자가 작을수록 높은 프레임, 낮을수록 낮은 프레임
	{
		float cellSize = static_cast<float>(WorldContext::I()->GetCellSize());
		if (diffLengthSquare < (std::pow(cellSize, 2.0f)))
		{
			return false;
		}

		pTransformComponent->SetPosition(m_destPos);
		DEFAULT_TRACE_LOG("목표지점까지 너무 멀어서 강제로 세팅!");
	}

	DEFAULT_TRACE_LOG("도달!");
	return true;
}