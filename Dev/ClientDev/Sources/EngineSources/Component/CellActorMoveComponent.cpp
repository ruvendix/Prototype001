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
	if (IsZeroMoveDirectionVector())
	{
		return false;
	}

	PawnActor* pOwner = dynamic_cast<PawnActor*>(GetOwner());
	ASSERT_LOG_RETURN_VALUE(pOwner != nullptr, false);

	Vector2d vActorNextPos = pOwner->BringPosition();
	Vector2d vMovePos = (m_vMoveDir * m_moveSpeed * deltaSeconds);
	vActorNextPos += vMovePos;

	pOwner->ApplyPosition(vActorNextPos);

	// �����ߴ��� Ȯ��
	if (CheckArrivedGoalPosition(deltaSeconds, vActorNextPos) == true)
	{
		ResetMoveDirectionVector();
		pOwner->ApplyCellPosition(m_destCellPos);
	}

	return true;
}

ComponentPtr CellActorMoveComponent::CreateClone()
{
	return std::make_shared<CellActorMoveComponent>(*this);
}

bool CellActorMoveComponent::ProcessMoveDirection(const Vector2d& vMoveDir, bool bMoveForward)
{
	PawnActor* pPawnActor = dynamic_cast<PawnActor*>(GetOwner());
	if (pPawnActor == nullptr)
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
		pPawnActor->ApplyMoveDirectionToLookAtDirection(vMoveDir);
	}

	// �̵� ��������?
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	if (pCurrentScene->CheckCanMoveToCellPosition(GetDestinationCellPosition(), pPawnActor) == false)
	{
		pPawnActor->ChangeActorStateDynamicSprite<PawnActorIdleState>();

		SetDestinationCellPosition(currentCellPos);
		ResetMoveDirectionVector();
		DEFAULT_TRACE_LOG("�̵� ����!");

		return false;
	}

	return true;
}

void CellActorMoveComponent::ProcessMoving()
{
	PawnActor* pPawnActor = dynamic_cast<PawnActor*>(GetOwner());
	if (pPawnActor == nullptr)
	{
		return;
	}

	// ��ǥ ������ �ٲ㼭 ����
	const Vector2d& vMoveDir = pPawnActor->CalculateMoveDirectionByCellPosition(pPawnActor->CalculateForwardCellPosition());
	ApplyMoveDirectionToDestination(vMoveDir);
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

bool CellActorMoveComponent::CheckArrivedGoalPosition(float deltaSeconds, const Vector2d& vCurrentPos) const
{
	// ���� �� ���� �Ÿ� ��
	const Vector2d& diffPos = (m_destPos - vCurrentPos);
	float diffLengthSquare = diffPos.CalculateLengthSquare();
	if (diffLengthSquare < (m_moveSpeed * deltaSeconds)) // �����̶�� �̰� ����
	{
		return true;
	}

	// ������ ����Ǿ��� ���� ��Ȳ (�� ������ �ִ� �Ÿ��� ��)
	float cellSize = static_cast<float>(WorldContext::I()->GetCellSize());
	if (diffLengthSquare > (std::pow(cellSize, 2.0f)))
	{
		return true;
	}

	//DEFAULT_TRACE_LOG("����!");
	return false;
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