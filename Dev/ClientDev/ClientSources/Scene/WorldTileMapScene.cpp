// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "WorldTileMapScene.h"

#include "EngineSources/Actor/WorldTileMapActor.h"
#include "ClientSources/World/WorldBackgroundActor.h"
#include "ClientSources/Editor/EditorActor.h"

WorldTileMapScene::WorldTileMapScene()
{

}

WorldTileMapScene::~WorldTileMapScene()
{

}

void WorldTileMapScene::Startup()
{
#pragma region ����� ó��
	// ����� ���� �߰�
	m_spWorldBackgroundActor = CreateActorToScene<WorldBackgroundActor>(EActorLayerType::WorldBackground);

	// �� ���� �ֱ�
	WorldContext::I()->SetCellSize(48);

	// �� ������ ���� Ÿ�ϸ� ����
	m_spWorldTileMapActor = CreateActorToScene<WorldTileMapActor>(EActorLayerType::WorldForeground);
#pragma endregion
}

bool WorldTileMapScene::CheckCanMoveToCellPosition(const Position2d& destCellPos, const Actor* pExcludeActor) const
{
	if (m_spWorldTileMapActor->CheckMovingAvailableTile(destCellPos) == false)
	{
		return false;
	}

	const ActorPtr& spFoundActor = FindCellActor(EActorLayerType::Creature, destCellPos, pExcludeActor);
	if (spFoundActor != nullptr)
	{
		return false;
	}

	return true;
}