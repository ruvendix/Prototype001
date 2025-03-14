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

void GameEntity::ApplyGameEntityMoveInfo(const Protocol::GameEntityInfo& gameEntityInfo)
{
	m_gameEntityInfo.set_cell_pos_x(gameEntityInfo.cell_pos_x());
	m_gameEntityInfo.set_cell_pos_y(gameEntityInfo.cell_pos_y());
}