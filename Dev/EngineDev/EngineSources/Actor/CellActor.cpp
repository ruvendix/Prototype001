// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CellActor.h"

#include "EngineSources/Component/CellActorMoveComponent.h"

Vector2d CellActor::ConvertCellPositionToWorldPosition(const Position2d& cellPos)
{
	return (ConvertCellPositionToWorldPosition(cellPos.x, cellPos.y));
}

Vector2d CellActor::ConvertCellPositionToWorldPosition(int32 x, int32 y)
{
	int32 cellSize = WorldContext::I()->GetCellSize();

	Vector2d convertedPos;
	convertedPos.x = static_cast<float>((x * cellSize) + (cellSize / 2));
	convertedPos.y = static_cast<float>((y * cellSize) + (cellSize / 2));

	return convertedPos;
}

Position2d CellActor::ConvertWorldPositionToCellPosition(const Vector2d& worldPos)
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

bool CellActor::CheckMovingState() const
{
	return false;
}

void CellActor::ApplyCellPosition(int32 x, int32 y)
{
	m_cellPos = Position2d{ x, y };

	const Vector2d& vWorldPos = ConvertCellPositionToWorldPosition(x, y);
	ApplyPosition(vWorldPos);
}

void CellActor::ApplyCellPosition(const Position2d& cellPos)
{
	ApplyCellPosition(cellPos.x, cellPos.y);
}

Position2d CellActor::ApplyRandomCellPosition()
{
	Position2d randomCellPos;

	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, randomCellPos);

	while (true)
	{
		const Position2d& tempRandomCellPos = WorldContext::I()->CalculateRandomCellPosition();
		if (pCurrentScene->CheckCanMoveToCellPosition(tempRandomCellPos, nullptr) == true)
		{
			ApplyCellPosition(tempRandomCellPos);
			randomCellPos = tempRandomCellPos;
			break;
		}
	}

	return randomCellPos;
}

Vector2d CellActor::CalculateMoveDirectionByCellPosition(const Position2d& destCellPos) const
{
	const Position2d& diffCellPos = (destCellPos - GetCellPosition());
	const Vector2d& vMoveDir{ static_cast<float>(diffCellPos.x),  static_cast<float>(diffCellPos.y) };
	return vMoveDir;
}

bool CellActor::CheckEqaulCellPosition(const Position2d& otherCellPos) const
{
	const Vector2d& vWorldPos = BringPosition();
	const Position2d& myCellPos = CellActor::ConvertWorldPositionToCellPosition(vWorldPos);
	return (myCellPos == otherCellPos);
}