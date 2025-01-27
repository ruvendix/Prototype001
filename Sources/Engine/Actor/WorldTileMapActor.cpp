// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WorldTileMapActor.h"

#include "WorldTileActor.h"

WorldTileMapActor::~WorldTileMapActor()
{
	// �ϴ��� ������ ���̺�
	FileStream writeWorldTileMapFileStream;
	writeWorldTileMapFileStream.OpenFileStream("WorldTileMap.wtm", EFileStreamMode::WriteOnlyBinary);
	SaveToFileStream(writeWorldTileMapFileStream);
}

void WorldTileMapActor::Startup()
{
	Super::Startup();

	int32 cellSize = WorldContext::I()->GetCellSize();

#if 0
	// ���� ũ�⸦ �� ���̸�ŭ ������ ���ο� ���� �������� �˾Ƴ���
	m_tileArray2dInfo.xIdxCount = (WorldContext::I()->GetWorldSize().width / cellSize);
	m_tileArray2dInfo.yIdxCount = (WorldContext::I()->GetWorldSize().height / cellSize);

	// Ÿ���� �� ���ͷ� �����! (�� ��ǥ�� �ʿ���)
	for (int32 y = 0; y < m_tileArray2dInfo.yIdxCount; ++y)
	{
		for (int32 x = 0; x < m_tileArray2dInfo.xIdxCount; ++x)
		{
			const std::shared_ptr<WorldTileActor> spWorldTileActor = Scene::CreateActor<WorldTileActor>();
			spWorldTileActor->InitializeWorldTile(x, y);

			AddChild(spWorldTileActor);
		}
	}
#else
	FileStream readWorldTileMapFileStream;
	readWorldTileMapFileStream.OpenFileStream("WorldTileMap.wtm", EFileStreamMode::ReadOnlyBinary);
	LoadFromFileStream(readWorldTileMapFileStream);
#endif
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

void WorldTileMapActor::SaveToFileStream(const FileStream& fileStream) const
{
	Super::SaveToFileStream(fileStream);

	fileStream.WriteDataToFileStream<Array2dInfo>(m_tileArray2dInfo);

	const std::string& strTileSpritePath = GetChild<WorldTileActor>(0)->BringTileSpritePath();
	fileStream.WriteStringToFileStream(strTileSpritePath);

	for (int32 y = 0; y < m_tileArray2dInfo.yIdxCount; ++y)
	{
		for (int32 x = 0; x < m_tileArray2dInfo.xIdxCount; ++x)
		{
			int32 worldTileIdx = ((y * m_tileArray2dInfo.xIdxCount) + x);
			GetChildNoCast(worldTileIdx)->SaveToFileStream(fileStream);
		}
	}
}

void WorldTileMapActor::LoadFromFileStream(const FileStream& fileStream)
{
	Super::LoadFromFileStream(fileStream);

	fileStream.ReadDataFromFileStream<Array2dInfo>(m_tileArray2dInfo);

	std::string strWorldTileSpritePath;
	fileStream.ReadStringFromFileStream(strWorldTileSpritePath);

	for (int32 y = 0; y < m_tileArray2dInfo.yIdxCount; ++y)
	{
		for (int32 x = 0; x < m_tileArray2dInfo.xIdxCount; ++x)
		{
			// Ÿ���� �� ���ͷ� �����! (�� ��ǥ�� �ʿ���)
			const std::shared_ptr<WorldTileActor> spWorldTileActor = Scene::CreateActor<WorldTileActor>();
			spWorldTileActor->LoadFromFileStream(fileStream);
			spWorldTileActor->InitializeWorldTile(x, y, strWorldTileSpritePath);

			AddChild(spWorldTileActor);
		}
	}
}

void WorldTileMapActor::ApplyCurrentMousePositionToTile()
{
	const Position2d& currentMousePos = MouseInputDevice::I()->GetCurrentMousePosition();
	if ((currentMousePos.x < 0) ||
		(currentMousePos.y < 0))
	{
		return;
	}

	// ���� ī�޶� ������ ��ǥ
	const std::shared_ptr<CameraActor>& spMainCameraActor = SceneRenderer::I()->GetMainCameraActor();
	const Vec2d& mainCameraOffsetPos = spMainCameraActor->BringCameraOffsetPosition();

	// ���� ��ǥ�� ��ȯ
	Vec2d mouseWorldPos;
	mouseWorldPos.x = static_cast<float>(currentMousePos.x) + mainCameraOffsetPos.x;
	mouseWorldPos.y = static_cast<float>(currentMousePos.y) + mainCameraOffsetPos.y;

	// ī�޶� ���� �ȿ� ���� ���� ����
	if (spMainCameraActor->CheckInCameraVision(mouseWorldPos) == false)
	{
		return;
	}
	
	const Position2d& mouseCellPos = CellActor::ConvertWorldPositionToCellPosition(mouseWorldPos);
	int32 findTileIdx = ConvertCellPositionToTileIndex(mouseCellPos);
	GetChild<WorldTileActor>(findTileIdx)->ApplyNextWorldTileShape();
}

bool WorldTileMapActor::CheckMovingAvailableTile(const Position2d& cellPos) const
{
	int32 findTileIdx = ConvertCellPositionToTileIndex(cellPos);
	int32 tileShapeIdx = GetChild<WorldTileActor>(findTileIdx)->GetTileShapeIdx();

	if (tileShapeIdx == 1)
	{
		return false;
	}

	return true;
}

int32 WorldTileMapActor::ConvertCellPositionToTileIndex(const Position2d& cellPos) const
{
	if ((cellPos.x) < 0 ||
		(cellPos.y) < 0)
	{
		return 0;
	}

	return (m_tileArray2dInfo.xIdxCount * cellPos.y) + cellPos.x;
}