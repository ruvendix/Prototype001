// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "CellActor.h"

#include "Engine/Component/SceneActorMoveComponent.h"

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

bool CellActor::PostUpdate(float deltaSeconds)
{
	return (Super::PostUpdate(deltaSeconds));
}

void CellActor::Cleanup()
{
	return (Super::Cleanup());
}

void CellActor::ApplyCellPosition(int32 cellPosX, int32 cellPosY)
{
	m_cellPos = CellPosition{ cellPosX, cellPosY };

	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetPosition(ConvertCellPositionToWorldPosition(cellPosX, cellPosY));
}

Vec2d CellActor::ConvertCellPositionToWorldPosition(const CellPosition& cellPos) const
{
	return (ConvertCellPositionToWorldPosition(cellPos.cellPosX, cellPos.cellPosY));
}

Vec2d CellActor::ConvertCellPositionToWorldPosition(int32 cellPosX, int32 cellPosY) const
{
	int32 cellSize = WorldContext::I()->GetCellSize();

	Vec2d convertedPos;
	convertedPos.x = static_cast<float>((cellPosX * cellSize) + (cellSize / 2));
	convertedPos.y = static_cast<float>((cellPosY * cellSize) + (cellSize / 2));

	return convertedPos;
}

CellPosition CellActor::ConvertWorldPositionToCellPosition(float posX, float posY) const
{
	int32 cellSize = WorldContext::I()->GetCellSize();

	CellPosition convertedCellPos;
	convertedCellPos.cellPosX = static_cast<int32>(posX) / cellSize;
	convertedCellPos.cellPosY = static_cast<int32>(posY) / cellSize;

	return convertedCellPos;
}