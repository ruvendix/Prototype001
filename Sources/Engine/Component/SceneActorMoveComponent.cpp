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
	// �Ǽ��ϱ� ��Ȯ�� üũ
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

	// ��ǥ ��ǥ ����ϱ�
	CellActor* pSceneActor = GetOwner<CellActor>();
	m_destPos = pSceneActor->ConvertCellPositionToWorldPosition(m_destCellPos);

	m_vMoveDir = vMoveDir;
}

bool SceneActorMoveComponent::CheckGoalPosition(float deltaSeconds) const
{
	TransformComponent* pTransformComponent = GetOwner()->BringTransformComponent();
	const Vec2d& currentPos = pTransformComponent->GetPosition();

	// ���� �� ���� �Ÿ� ��
	const Vec2d& diffPos = (m_destPos - currentPos);
	float diffLengthSquare = diffPos.CalculateLengthSquare();
	if (diffLengthSquare > (m_moveSpeed * deltaSeconds)) // ���ڰ� �������� ���� ������, �������� ���� ������
	{
		float cellSize = static_cast<float>(WorldContext::I()->GetCellSize());
		if (diffLengthSquare < (std::pow(cellSize, 2.0f)))
		{
			return false;
		}

		pTransformComponent->SetPosition(m_destPos);
		DEFAULT_TRACE_LOG("��ǥ�������� �ʹ� �־ ������ ����!");
	}

	DEFAULT_TRACE_LOG("����!");
	return true;
}

void SceneActorMoveComponent::ResetMoveDirection()
{
	m_vMoveDir = Vec2d();
}