// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonster.h"

#include "GameMonsterActionState.h"

GameMonster::GameMonster()
{

}

GameMonster::~GameMonster()
{

}

void GameMonster::Startup()
{
	m_spActionState = std::make_shared<GameMonsterIdleState>(SharedFromThisExactType<GameMonster>());
	m_spActionState->Startup();
}

bool GameMonster::Update(float deltaSeconds)
{
	m_reserveNextStateEvent.ExcuteIfBound();
	m_spActionState->Update(deltaSeconds);

	return true;
}

void GameMonster::Cleanup()
{

}

void GameMonster::ApplyGameMonsterInfo(const Protocol::GameMonsterInfo& srcGameMonsterInfo)
{
	m_monsterId = srcGameMonsterInfo.monster_id();
	SetGameEntityInfo(srcGameMonsterInfo.entity_info());
}

void GameMonster::CopyGameMonsterInfo(Protocol::GameMonsterInfo* pDestGameMonsterInfo)
{
	pDestGameMonsterInfo->set_monster_id(m_monsterId);
	*(pDestGameMonsterInfo->mutable_entity_info()) = GetGameEntityInfo();
}