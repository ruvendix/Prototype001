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

bool SceneActorMoveComponent::Update(float deltaSeconds)
{
	TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	Vec2d vActorNextPos = pTransformComponent->GetPosition();

	Vec2d vMovePos = (m_vMoveDir * m_moveSpeed * deltaSeconds);
	vActorNextPos += vMovePos;

	pTransformComponent->SetPosition(vActorNextPos);
	return true;
}

void SceneActorMoveComponent::ApplyMoveDirectionVector(const Vec2d& vMoveDir)
{
	// 실수니까 정확히 체크
	if (math::CheckAlikeValue(vMoveDir.x, 1.0f) == true)
	{
		m_lookAtType = ESceneActorLookAtType::Right;
	}
	else if (math::CheckAlikeValue(vMoveDir.x, -1.0f) == true)
	{
		m_lookAtType = ESceneActorLookAtType::Left;
	}

	if (math::CheckAlikeValue(vMoveDir.y, 1.0f) == true)
	{
		m_lookAtType = ESceneActorLookAtType::Down;
	}
	else if (math::CheckAlikeValue(vMoveDir.y, -1.0f) == true)
	{
		m_lookAtType = ESceneActorLookAtType::Up;
	}

	m_destCellPos.x += static_cast<int32>(vMoveDir.x);
	m_destCellPos.y += static_cast<int32>(vMoveDir.y);

	// 목표 좌표 계산하기
	CellActor* pSceneActor = GetOwner<CellActor>();
	m_destPos = pSceneActor->ConvertCellPositionToWorldPosition(m_destCellPos);

	m_vMoveDir = vMoveDir;
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

void SceneActorMoveComponent::ResetMoveDirection()
{
	m_vMoveDir = Vec2d();
}