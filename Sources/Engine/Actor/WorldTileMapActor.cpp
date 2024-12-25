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

	// ���� ũ�⸦ �� ���̸�ŭ ������ ���ο� ���� �������� �˾Ƴ���
	Array2dInfo tileArray2dInfo;
	tileArray2dInfo.xIdxCount = (WorldContext::I()->GetWorldSize().width / cellSize);
	tileArray2dInfo.yIdxCount = (WorldContext::I()->GetWorldSize().height / cellSize);

	// Ÿ���� �� ���ͷ� �����! (�� ��ǥ�� �ʿ���)
	for (int32 y = 0; y < tileArray2dInfo.yIdxCount; ++y)
	{
		for (int32 x = 0; x < tileArray2dInfo.xIdxCount; ++x)
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
