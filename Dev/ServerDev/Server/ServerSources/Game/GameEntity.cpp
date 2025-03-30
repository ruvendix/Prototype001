// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameEntity.h"

const std::array<Position2d, TO_NUM(Protocol::EGameEntityLookAtDir::Count)> g_lookAtForwardCellPosTable =
{
	Position2d(-1, +0),
	Position2d(+0, -1),
	Position2d(+1, +0),
	Position2d(+0, +1),
};

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
	m_gameEntityInfo.set_entitye_look_at_dir(CalculateGameEntityLookAtDirection(gameEntityInfo));
	m_gameEntityInfo.set_cell_pos_x(gameEntityInfo.cell_pos_x());
	m_gameEntityInfo.set_cell_pos_y(gameEntityInfo.cell_pos_y());
}

void GameEntity::ApplyGameEntityState(const Protocol::GameEntityInfo& gameEntityInfo)
{
	m_gameEntityInfo.set_entity_state(gameEntityInfo.entity_state());
}

bool GameEntity::CheckGameEntityState(Protocol::EGameEntityState gameEntityState) const
{
	return (m_gameEntityInfo.entity_state() == gameEntityState);
}

bool GameEntity::CheckSameCellPosition(const Position2d& targetCellPos) const
{
	const Position2d& currentCellPos = { static_cast<int32>(m_gameEntityInfo.cell_pos_x()), static_cast<int32>(m_gameEntityInfo.cell_pos_y()) };
	return (currentCellPos == targetCellPos);
}

Position2d GameEntity::MakeCurrentCellPosition() const
{
	const Protocol::GameEntityInfo& gameEntityInfo = GetGameEntityInfo();

	Position2d currentCellPos;
	currentCellPos.x = static_cast<int32>(gameEntityInfo.cell_pos_x());
	currentCellPos.y = static_cast<int32>(gameEntityInfo.cell_pos_y());
	return currentCellPos;
}

Position2d GameEntity::MakeForwardCellPosition() const
{
	const Protocol::GameEntityInfo& gameEntityInfo = GetGameEntityInfo();
	Position2d forwardCellPos = (MakeCurrentCellPosition() + PawnActor::g_lookAtForwardCellPosTable[TO_NUM(gameEntityInfo.entitye_look_at_dir())]);
	return forwardCellPos;
}

Protocol::EGameEntityLookAtDir GameEntity::CalculateGameEntityLookAtDirection(const Position2d& destCellPos)
{
	const Position2d& currentCellPos = MakeCurrentCellPosition();

	Protocol::EGameEntityLookAtDir gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Count;
	const Position2d& diffCellPos = (destCellPos - currentCellPos);
	if (diffCellPos.x >= 1)
	{
		gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Right;
	}
	else if (diffCellPos.x <= -1)
	{
		gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Left;
	}
	else if (diffCellPos.y >= 1)
	{
		gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Down;
	}
	else if (diffCellPos.y <= -1)
	{
		gameEntityLookAtDir = Protocol::EGameEntityLookAtDir::Up;
	}

	return gameEntityLookAtDir;
}

Protocol::EGameEntityLookAtDir GameEntity::CalculateGameEntityLookAtDirection(const Protocol::GameEntityInfo& modifiedGameEntityInfo)
{
	const Position2d& destCellPos =
	{
		static_cast<int32>(modifiedGameEntityInfo.cell_pos_x()),
		static_cast<int32>(modifiedGameEntityInfo.cell_pos_y())
	};

	return (CalculateGameEntityLookAtDirection(destCellPos));
}