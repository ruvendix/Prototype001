// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneActor.h"

#include "Engine/Component/SceneActorMoveComponent.h"

SceneActor::~SceneActor()
{
	Cleanup();
}

void SceneActor::Startup()
{
	Super::Startup();
	AddComponent<SceneActorMoveComponent>();
}

bool SceneActor::Update(float deltaSeconds)
{
	return (Super::Update(deltaSeconds));
}

bool SceneActor::PostUpdate(float deltaSeconds)
{
	return (Super::PostUpdate(deltaSeconds));
}

void SceneActor::Cleanup()
{
	return (Super::Cleanup());
}

void SceneActor::ApplyCellPosition(int32 cellPosX, int32 cellPosY)
{
	m_cellPos = CellPosition{ cellPosX, cellPosY };

	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetPosition(ConvertCellPositionToWorldPosition(cellPosX, cellPosY));
}

// 이건 게임 규격이니까 글로벌한 값임
static const int32 CELL_SIZE = 48;

Vec2d SceneActor::ConvertCellPositionToWorldPosition(const CellPosition& cellPos) const
{
	return (ConvertCellPositionToWorldPosition(cellPos.cellPosX, cellPos.cellPosY));
}

Vec2d SceneActor::ConvertCellPositionToWorldPosition(int32 cellPosX, int32 cellPosY) const
{
	Vec2d convertedPos;
	convertedPos.x = static_cast<float>((cellPosX * CELL_SIZE) + (CELL_SIZE / 2));
	convertedPos.y = static_cast<float>((cellPosY * CELL_SIZE) + (CELL_SIZE / 2));

	return convertedPos;
}

CellPosition SceneActor::ConvertWorldPositionToCellPosition(float posX, float posY) const
{
	CellPosition convertedCellPos;
	convertedCellPos.cellPosX = static_cast<int32>(posX) / CELL_SIZE;
	convertedCellPos.cellPosY = static_cast<int32>(posY) / CELL_SIZE;

	return convertedCellPos;
}