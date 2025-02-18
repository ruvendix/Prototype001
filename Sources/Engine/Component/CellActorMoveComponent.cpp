// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CellActorMoveComponent.h"

DEFINE_COMPILETIME_ID(CellActorMoveComponent, ComponentIdCounter)

CellActorMoveComponent::CellActorMoveComponent()
{

}

CellActorMoveComponent::~CellActorMoveComponent()
{

}

bool CellActorMoveComponent::Update(float deltaSeconds)
{
	Actor* pOwner = GetOwner();
	ASSERT_LOG_RETURN_VALUE(pOwner != nullptr, false);

	Vector2d vActorNextPos = pOwner->BringPosition();
	Vector2d vMovePos = (m_vMoveDir * m_moveSpeed * deltaSeconds);
	vActorNextPos += vMovePos;

	pOwner->ApplyPosition(vActorNextPos);
	return true;
}

ComponentPtr CellActorMoveComponent::CreateClone()
{
	return std::make_shared<CellActorMoveComponent>(*this);
}

bool CellActorMoveComponent::ProcessMoveDirection(const Vector2d& vMoveDir, bool bMoveForward)
{
	const std::shared_ptr<PawnActor>& spPawnActor = std::dynamic_pointer_cast<PawnActor>(GetOwner()->shared_from_this());
	if (spPawnActor == nullptr)
	{
		return false;
	}

	// ���� �� ��ǥ ���
	Position2d currentCellPos = m_destCellPos;

	// ��ǥ ���� �ٲٰ�
	ApplyMoveDirectionToDestination(vMoveDir);

	// �������� �̵��� ���� ���� ������ �ٲ�
	if (bMoveForward == true)
	{
		spPawnActor->ApplyMoveDirectionToLookAtDirection(vMoveDir);
	}

	// �̵� ��������?
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	if (pCurrentScene->CheckCanMoveToCellPosition(GetDestinationCellPosition(), spPawnActor) == false)
	{
		spPawnActor->ChangeActorStateDynamicSprite<PawnActorIdleState>();

		SetDestinationCellPosition(currentCellPos);
		ResetMoveDirectionVector();
		DEFAULT_TRACE_LOG("�̵� ����!");

		return false;
	}

	return true;
}

void CellActorMoveComponent::ApplyMoveDirectionToDestination(const Vector2d& vMoveDir)
{
	m_destCellPos.x += static_cast<int32>(vMoveDir.x);
	m_destCellPos.y += static_cast<int32>(vMoveDir.y);

	// ��ǥ ��ǥ ����ϱ�
	CellActor* pCellActor = GetOwner<CellActor>();
	if (pCellActor == nullptr)
	{
		return;
	}

	m_destPos = pCellActor->ConvertCellPositionToWorldPosition(m_destCellPos);
	m_vMoveDir = vMoveDir;
}

bool CellActorMoveComponent::TryCheckValidateGoalPosition(float deltaSeconds, bool bForceApplyGoalPos) const
{
	Actor* pOwner = GetOwner();
	ASSERT_LOG_RETURN_VALUE(pOwner != nullptr, false);
	const Vector2d& currentPos = pOwner->BringPosition();

	// ���� �� ���� �Ÿ� ��
	const Vector2d& diffPos = (m_destPos - currentPos);
	float diffLengthSquare = diffPos.CalculateLengthSquare();
	if (diffLengthSquare > (m_moveSpeed * deltaSeconds)) // ���ڰ� �������� ���� ������, �������� ���� ������
	{
		float cellSize = static_cast<float>(WorldContext::I()->GetCellSize());
		if (diffLengthSquare < (std::pow(cellSize, 2.0f)))
		{
			return false;
		}

		if (bForceApplyGoalPos == true)
		{
			pOwner->ApplyPosition(m_destPos);
			DEFAULT_TRACE_LOG("��ǥ�������� �ʹ� �־ ������ ����!");
		}
	}
	
	//DEFAULT_TRACE_LOG("����!");
	return true;
}

void CellActorMoveComponent::ResetMoveDirectionVector()
{
	m_vMoveDir = Vector2d();
}

void CellActorMoveComponent::ApplyDestinationDataToOwner()
{
	CellActor* pOwner = dynamic_cast<CellActor*>(GetOwner());
	if (pOwner == nullptr)
	{
		return;
	}

	pOwner->ApplyPosition(m_destPos);
	pOwner->SetCellPosition(m_destCellPos);
}