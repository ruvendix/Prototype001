// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameEntity.h"

GameEntity::GameEntity()
{

}

GameEntity::~GameEntity()
{

}

void GameEntity::Startup()
{

}

void GameEntity::Cleanup()
{

}

void GameEntity::ApplyGameEntityLookAtDirection(const Protocol::GameEntityInfo& gameEntityInfo)
{
	m_gameEntityInfo.set_entitye_look_at_dir(gameEntityInfo.entitye_look_at_dir());
}

void GameEntity::ApplyGameEntityMoveInfo(const Protocol::GameEntityInfo& gameEntityInfo)
{
	m_gameEntityInfo.set_entity_state(gameEntityInfo.entity_state());
	m_gameEntityInfo.set_entitye_look_at_dir(CalculateActorLookAtDirection(gameEntityInfo));
	m_gameEntityInfo.set_cell_pos_x(gameEntityInfo.cell_pos_x());
	m_gameEntityInfo.set_cell_pos_y(gameEntityInfo.cell_pos_y());
}

void GameEntity::ApplyGameEntityState(const Protocol::GameEntityInfo& gameEntityInfo)
{
	m_gameEntityInfo.set_entity_state(gameEntityInfo.entity_state());
}

Protocol::EGameEntityLookAtDir GameEntity::CalculateActorLookAtDirection(const Protocol::GameEntityInfo& modifiedGameEntityInfo)
{
	const Position2d& currentCellPos = { static_cast<int32>(m_gameEntityInfo.cell_pos_x()), static_cast<int32>(m_gameEntityInfo.cell_pos_y()) };
	const Position2d& destCellPos = { static_cast<int32>(modifiedGameEntityInfo.cell_pos_x()), static_cast<int32>(modifiedGameEntityInfo.cell_pos_y()) };

	Protocol::EGameEntityLookAtDir gameEntityeLookAtDir = Protocol::EGameEntityLookAtDir::Count;
	const Position2d& diffCellPos = (destCellPos - currentCellPos);
	if (diffCellPos.x == 1)
	{
		gameEntityeLookAtDir = Protocol::EGameEntityLookAtDir::Right;
	}
	else if (diffCellPos.x == -1)
	{
		gameEntityeLookAtDir = Protocol::EGameEntityLookAtDir::Left;
	}
	else if (diffCellPos.y == 1)
	{
		gameEntityeLookAtDir = Protocol::EGameEntityLookAtDir::Down;
	}
	else if (diffCellPos.y == -1)
	{
		gameEntityeLookAtDir = Protocol::EGameEntityLookAtDir::Up;
	}

	return gameEntityeLookAtDir;
}