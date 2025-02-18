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

	// 현재 셀 좌표 백업
	Position2d currentCellPos = m_destCellPos;

	// 목표 지점 바꾸고
	ApplyMoveDirectionToDestination(vMoveDir);

	// 전방으로 이동할 때만 보는 방향을 바꿈
	if (bMoveForward == true)
	{
		spPawnActor->ApplyMoveDirectionToLookAtDirection(vMoveDir);
	}

	// 이동 가능한지?
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	if (pCurrentScene->CheckCanMoveToCellPosition(GetDestinationCellPosition(), spPawnActor) == false)
	{
		spPawnActor->ChangeActorStateDynamicSprite<PawnActorIdleState>();

		SetDestinationCellPosition(currentCellPos);
		ResetMoveDirectionVector();
		DEFAULT_TRACE_LOG("이동 못함!");

		return false;
	}

	return true;
}

void CellActorMoveComponent::ApplyMoveDirectionToDestination(const Vector2d& vMoveDir)
{
	m_destCellPos.x += static_cast<int32>(vMoveDir.x);
	m_destCellPos.y += static_cast<int32>(vMoveDir.y);

	// 목표 좌표 계산하기
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

	// 이제 그 둘의 거리 비교
	const Vector2d& diffPos = (m_destPos - currentPos);
	float diffLengthSquare = diffPos.CalculateLengthSquare();
	if (diffLengthSquare > (m_moveSpeed * deltaSeconds)) // 숫자가 작을수록 높은 프레임, 낮을수록 낮은 프레임
	{
		float cellSize = static_cast<float>(WorldContext::I()->GetCellSize());
		if (diffLengthSquare < (std::pow(cellSize, 2.0f)))
		{
			return false;
		}

		if (bForceApplyGoalPos == true)
		{
			pOwner->ApplyPosition(m_destPos);
			DEFAULT_TRACE_LOG("목표지점까지 너무 멀어서 강제로 설정!");
		}
	}
	
	//DEFAULT_TRACE_LOG("도달!");
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