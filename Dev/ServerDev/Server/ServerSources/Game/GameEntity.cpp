// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameEntity.h"

const std::array<Position2d, Protocol::EEntityLookAtDirection_ARRAYSIZE> g_lookAtForwardCellPosTable =
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

void GameEntity::AddToSyncEntitiesPacket(Protocol::S_SyncEntitiesPacket& outSyncEntitiesPacket)
{
	
}

void GameEntity::ApplyGameEntityLookAtDirection(const Protocol::EntityInfo& entityInfo)
{
	m_entityInfo.set_entity_look_at_dir(entityInfo.entity_look_at_dir());
}

void GameEntity::ApplyGameEntityMoveInfo(const Protocol::EntityInfo& entityInfo)
{
	m_entityInfo.set_entity_state(entityInfo.entity_state());
	m_entityInfo.set_entity_look_at_dir(CalculateGameEntityLookAtDirection(entityInfo));
	m_entityInfo.set_cell_pos_x(entityInfo.cell_pos_x());
	m_entityInfo.set_cell_pos_y(entityInfo.cell_pos_y());
}

void GameEntity::ApplyGameEntityState(Protocol::EEntityState entityState)
{
	m_entityInfo.set_entity_state(entityState);
}

void GameEntity::ApplyGameEntityHp(int32 entityHp)
{
	m_entityInfo.set_hp(entityHp);
}

bool GameEntity::CheckGameEntityState(Protocol::EEntityState entityState) const
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
	const Protocol::EntityInfo& entityInfo = GetGameEntityInfo();

	Position2d currentCellPos;
	currentCellPos.x = static_cast<int32>(entityInfo.cell_pos_x());
	currentCellPos.y = static_cast<int32>(entityInfo.cell_pos_y());
	return currentCellPos;
}

Position2d GameEntity::MakeForwardCellPosition() const
{
	const Protocol::EntityInfo& entityInfo = GetGameEntityInfo();
	Position2d forwardCellPos = (MakeCurrentCellPosition() + PawnActor::g_lookAtForwardCellPosTable[TO_NUM(entityInfo.entity_look_at_dir())]);
	return forwardCellPos;
}

Protocol::EEntityLookAtDirection GameEntity::CalculateGameEntityLookAtDirection(const Position2d& destCellPos) const
{
	const Position2d& currentCellPos = MakeCurrentCellPosition();

	Protocol::EEntityLookAtDirection entityLookAtDir = Protocol::EEntityLookAtDirection::Down;
	const Position2d& diffCellPos = (destCellPos - currentCellPos);
	if (diffCellPos.x >= 1)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Right;
	}
	else if (diffCellPos.x <= -1)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Left;
	}
	else if (diffCellPos.y >= 1)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Down;
	}
	else if (diffCellPos.y <= -1)
	{
		entityLookAtDir = Protocol::EEntityLookAtDirection::Up;
	}

	return entityLookAtDir;
}

Protocol::EEntityLookAtDirection GameEntity::CalculateGameEntityLookAtDirection(const Protocol::EntityInfo& targetEntityInfo) const
{
	const Position2d& destCellPos =
	{
		static_cast<int32>(targetEntityInfo.cell_pos_x()),
		static_cast<int32>(targetEntityInfo.cell_pos_y())
	};

	return (CalculateGameEntityLookAtDirection(destCellPos));
}