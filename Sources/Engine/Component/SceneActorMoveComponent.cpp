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
	return true;
}

void SceneActorMoveComponent::ApplyMoveDirectionVector(const Vec2d& vMoveDir)
{
	// �Ǽ��ϱ� ��Ȯ�� üũ
	if (math::CheckAlikeValue(vMoveDir.x, 1.0f) == true)
	{
		m_moveDir = ESceneActorMoveDirection::Right;
	}
	else if (math::CheckAlikeValue(vMoveDir.x, -1.0f) == true)
	{
		m_moveDir = ESceneActorMoveDirection::Left;
	}

	if (math::CheckAlikeValue(vMoveDir.y, 1.0f) == true)
	{
		m_moveDir = ESceneActorMoveDirection::Down;
	}
	else if (math::CheckAlikeValue(vMoveDir.y, -1.0f) == true)
	{
		m_moveDir = ESceneActorMoveDirection::Up;
	}

	m_destCellPos.x += static_cast<int32>(vMoveDir.x);
	m_destCellPos.y += static_cast<int32>(vMoveDir.y);

	// ��ǥ ��ǥ ����ϱ�
	CellActor* pSceneActor = GetOwner<CellActor>();
	m_destPos = pSceneActor->ConvertCellPositionToWorldPosition(m_destCellPos);
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