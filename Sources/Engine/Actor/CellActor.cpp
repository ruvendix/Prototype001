// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CellActor.h"

#include "Engine/Component/CellActorMoveComponent.h"

Vec2d CellActor::ConvertCellPositionToWorldPosition(const Position2d& cellPos)
{
	return (ConvertCellPositionToWorldPosition(cellPos.x, cellPos.y));
}

Vec2d CellActor::ConvertCellPositionToWorldPosition(int32 x, int32 y)
{
	int32 cellSize = WorldContext::I()->GetCellSize();

	Vec2d convertedPos;
	convertedPos.x = static_cast<float>((x * cellSize) + (cellSize / 2));
	convertedPos.y = static_cast<float>((y * cellSize) + (cellSize / 2));

	return convertedPos;
}

Position2d CellActor::ConvertWorldPositionToCellPosition(const Vec2d& worldPos)
{
	return (ConvertWorldPositionToCellPosition(worldPos.x, worldPos.y));
}

Position2d CellActor::ConvertWorldPositionToCellPosition(float posX, float posY)
{
	int32 cellSize = WorldContext::I()->GetCellSize();

	Position2d convertedCellPos;
	convertedCellPos.x = static_cast<int32>(posX) / cellSize;
	convertedCellPos.y = static_cast<int32>(posY) / cellSize;

	return convertedCellPos;
}

CellActor::~CellActor()
{
	Cleanup();
}

void CellActor::Startup()
{
	Super::Startup();
}

bool CellActor::Update(float deltaSeconds)
{
	return (Super::Update(deltaSeconds));
}

void CellActor::Cleanup()
{
	return (Super::Cleanup());
}

void CellActor::ApplyCellPosition(int32 x, int32 y)
{
	m_cellPos = Position2d{ x, y };

	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetPosition(ConvertCellPositionToWorldPosition(x, y));
}

void CellActor::ApplyCellPosition(const Position2d& cellPos)
{
	ApplyCellPosition(cellPos.x, cellPos.y);
}

void CellActor::ApplyRandomCellPosition()
{
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	while (true)
	{
		const Position2d& randomCellPos = WorldContext::I()->CalculateRandomCellPosition();
		if (pCurrentScene->CheckCanMoveToCellPosition(randomCellPos) == true)
		{
			ApplyCellPosition(randomCellPos);
			return;
		}
	}
}

bool CellActor::CheckEqaulCellPosition(const Position2d& otherCellPos) const
{
	const TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG_RETURN_VALUE(pTransformComponent != nullptr, false);
	const Position2d& myCellPos = CellActor::ConvertWorldPositionToCellPosition(pTransformComponent->GetPosition());
	return (myCellPos == otherCellPos);
}