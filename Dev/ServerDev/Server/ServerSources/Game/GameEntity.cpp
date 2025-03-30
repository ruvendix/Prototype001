// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameEntity.h"

const std::array<Position2d, TO_NUM(Protocol::ENetworkEntityLookAtDirection::Count)> g_lookAtForwardCellPosTable =
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

void GameEntity::ApplyGameEntityLookAtDirection(const Protocol::NetworkEntityInfo& entityInfo)
{
	m_entityInfo.set_entitye_look_at_dir(entityInfo.entitye_look_at_dir());
}

void GameEntity::ApplyGameEntityMoveInfo(const Protocol::NetworkEntityInfo& entityInfo)
{
	m_entityInfo.set_entity_state(entityInfo.entity_state());
	m_entityInfo.set_entitye_look_at_dir(CalculateGameEntityLookAtDirection(entityInfo));
	m_entityInfo.set_cell_pos_x(entityInfo.cell_pos_x());
	m_entityInfo.set_cell_pos_y(entityInfo.cell_pos_y());
}

void GameEntity::ApplyGameEntityState(const Protocol::NetworkEntityInfo& entityInfo)
{
	m_entityInfo.set_entity_state(entityInfo.entity_state());
}

bool GameEntity::CheckGameEntityState(Protocol::ENetworkEntityState entityState) const
{
	return (m_entityInfo.entity_state() == entityState);
}

bool GameEntity::CheckSameCellPosition(const Position2d& targetCellPos) const
{
	const Position2d& currentCellPos = { static_cast<int32>(m_entityInfo.cell_pos_x()), static_cast<int32>(m_entityInfo.cell_pos_y()) };
	return (currentCellPos == targetCellPos);
}

Position2d GameEntity::MakeCurrentCellPosition() const
{
	const Protocol::NetworkEntityInfo& entityInfo = GetGameEntityInfo();

	Position2d currentCellPos;
	currentCellPos.x = static_cast<int32>(entityInfo.cell_pos_x());
	currentCellPos.y = static_cast<int32>(entityInfo.cell_pos_y());
	return currentCellPos;
}

Position2d GameEntity::MakeForwardCellPosition() const
{
	const Protocol::NetworkEntityInfo& entityInfo = GetGameEntityInfo();
	Position2d forwardCellPos = (MakeCurrentCellPosition() + PawnActor::g_lookAtForwardCellPosTable[TO_NUM(entityInfo.entitye_look_at_dir())]);
	return forwardCellPos;
}

Protocol::ENetworkEntityLookAtDirection GameEntity::CalculateGameEntityLookAtDirection(const Position2d& destCellPos) const
{
	const Position2d& currentCellPos = MakeCurrentCellPosition();

	Protocol::ENetworkEntityLookAtDirection entityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Count;
	const Position2d& diffCellPos = (destCellPos - currentCellPos);
	if (diffCellPos.x >= 1)
	{
		entityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Right;
	}
	else if (diffCellPos.x <= -1)
	{
		entityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Left;
	}
	else if (diffCellPos.y >= 1)
	{
		entityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Down;
	}
	else if (diffCellPos.y <= -1)
	{
		entityLookAtDir = Protocol::ENetworkEntityLookAtDirection::Up;
	}

	return entityLookAtDir;
}

Protocol::ENetworkEntityLookAtDirection GameEntity::CalculateGameEntityLookAtDirection(const Protocol::NetworkEntityInfo& targetEntityInfo) const
{
	const Position2d& destCellPos =
	{
		static_cast<int32>(targetEntityInfo.cell_pos_x()),
		static_cast<int32>(targetEntityInfo.cell_pos_y())
	};

	return (CalculateGameEntityLookAtDirection(destCellPos));
}