// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonster.h"

GameMonster::GameMonster()
{

}

GameMonster::~GameMonster()
{

}

void GameMonster::Startup()
{

}

void GameMonster::Cleanup()
{

}

void GameMonster::ApplyGameMonsterInfo(const Protocol::GameMonsterInfo& srcGameMonsterInfo)
{
	m_monsterId = srcGameMonsterInfo.monster_id();
	SetGameEntityInfo(srcGameMonsterInfo.game_entity_info());
}

void GameMonster::CopyGameMonsterInfo(Protocol::GameMonsterInfo* pDestGameMonsterInfo)
{
	pDestGameMonsterInfo->set_monster_id(m_monsterId);
	*(pDestGameMonsterInfo->mutable_game_entity_info()) = GetGameEntityInfo();
}