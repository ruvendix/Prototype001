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

	Vec2d vActorNextPos = pOwner->BringPosition();
	Vec2d vMovePos = (m_vMoveDir * m_moveSpeed * deltaSeconds);
	vActorNextPos += vMovePos;

	pOwner->ApplyPosition(vActorNextPos);
	return true;
}

ComponentPtr CellActorMoveComponent::CreateClone()
{
	return std::make_shared<CellActorMoveComponent>(*this);
}

bool CellActorMoveComponent::ProcessMove(const Vec2d& vMoveDir)
{
	AnimationActor* pAnimationActor = dynamic_cast<AnimationActor*>(GetOwner());
	if (pAnimationActor == nullptr)
	{
		return false;
	}

	// 현재 셀 좌표 백업
	Position2d currentCellPos = m_destCellPos;

	// 목표 지점 바꾸고
	ApplyMoveDirectionToLookAtDirection(vMoveDir);

	// 보는 방향 바꾸고
	pAnimationActor->ApplyMoveDirectionToLookAtDirection(vMoveDir);

	// 이동 가능한지?
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
	if (pCurrentScene->CheckCanMoveToCellPosition(GetDestinationCellPosition()) == false)
	{
		pAnimationActor->ChangeActorStateDynamicSprite<AnimationActorIdleState>();

		SetDestinationCellPosition(currentCellPos);
		ResetMoveDirection();
		DEFAULT_TRACE_LOG("이동 못함!");

		return false;
	}

	return true;
}

void CellActorMoveComponent::ApplyMoveDirectionToLookAtDirection(const Vec2d& vMoveDir)
{
	m_destCellPos.x += static_cast<int32>(vMoveDir.x);
	m_destCellPos.y += static_cast<int32>(vMoveDir.y);

	// 목표 좌표 계산하기
	CellActor* pSceneActor = GetOwner<CellActor>();
	m_destPos = pSceneActor->ConvertCellPositionToWorldPosition(m_destCellPos);

	m_vMoveDir = vMoveDir;
}

bool CellActorMoveComponent::CheckGoalPosition(float deltaSeconds) const
{
	Actor* pOwner = GetOwner();
	ASSERT_LOG_RETURN_VALUE(pOwner != nullptr, false);
	const Vec2d& currentPos = pOwner->BringPosition();

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

		pOwner->ApplyPosition(m_destPos);
		DEFAULT_TRACE_LOG("목표지점까지 너무 멀어서 강제로 세팅!");
	}
	
	DEFAULT_TRACE_LOG("도달!");
	return true;
}

void CellActorMoveComponent::ResetMoveDirection()
{
	m_vMoveDir = Vec2d();
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