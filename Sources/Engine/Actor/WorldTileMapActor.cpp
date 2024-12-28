// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WorldTileMapActor.h"

#include "WorldTileActor.h"

WorldTileMapActor::~WorldTileMapActor()
{

}

void WorldTileMapActor::Startup()
{
	Super::Startup();

	int32 cellSize = WorldContext::I()->GetCellSize();

	// 월드 크기를 셀 길이만큼 나눠서 가로와 세로 개수부터 알아내기
	m_tileArray2dInfo.xIdxCount = (WorldContext::I()->GetWorldSize().width / cellSize);
	m_tileArray2dInfo.yIdxCount = (WorldContext::I()->GetWorldSize().height / cellSize);

	// 타일은 씬 액터로 만들기! (셀 좌표가 필요함)
	for (int32 y = 0; y < m_tileArray2dInfo.yIdxCount; ++y)
	{
		for (int32 x = 0; x < m_tileArray2dInfo.xIdxCount; ++x)
		{
			const std::shared_ptr<WorldTileActor> spWorldTileActor = Scene::CreateActor<WorldTileActor>();
			spWorldTileActor->InitializeWorldTile(x, y);

			AddChild(spWorldTileActor);
		}
	}
}

bool WorldTileMapActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void WorldTileMapActor::Cleanup()
{
	Super::Cleanup();
}

void WorldTileMapActor::ApplyCurrentMousePositionToTile()
{
	const Position2d& currentMousePos = MouseInputHandler::I()->GetCurrentMousePosition();
	if ((currentMousePos.x < 0) ||
		(currentMousePos.y < 0))
	{
		return;
	}

	// 메인 카메라 오프셋 좌표
	const Vec2d& mainCameraOffsetPos = SceneRenderer::I()->GetMainCameraActor()->BringCameraOffsetPosition();

	// 월드 좌표로 변환
	Vec2d mouseWorldPos;
	mouseWorldPos.x = static_cast<float>(currentMousePos.x) + mainCameraOffsetPos.x;
	mouseWorldPos.y = static_cast<float>(currentMousePos.y) + mainCameraOffsetPos.y;
	
	const CellPosition& mouseCellPos = CellActor::ConvertWorldPositionToCellPosition(mouseWorldPos);
	int32 findTileIdx = ConvertCellPositionToTileIndex(mouseCellPos);
	GetChild<WorldTileActor>(findTileIdx)->ApplyNextShape();
}

bool WorldTileMapActor::CheckMovingAvailableTile(const CellPosition& cellPos) const
{
	int32 findTileIdx = ConvertCellPositionToTileIndex(cellPos);
	int32 tileShapeIdx = GetChild<WorldTileActor>(findTileIdx)->GetTileShapeIdx();

	if (tileShapeIdx == 1)
	{
		return false;
	}

	return true;
}

int32 WorldTileMapActor::ConvertCellPositionToTileIndex(const CellPosition& cellPos) const
{
	if ((cellPos.cellPosX) < 0 ||
		(cellPos.cellPosY) < 0)
	{
		return 0;
	}

	return (m_tileArray2dInfo.xIdxCount * cellPos.cellPosY) + cellPos.cellPosX;
}