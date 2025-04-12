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

void GameMonster::AddToSyncEntitiesPacket(Protocol::S_SyncEntitiesPacket& outSyncEntitiesPacket)
{
	Protocol::MonsterInfo* pMonsterInfo = outSyncEntitiesPacket.add_monsters_info();
	pMonsterInfo->set_monster_id(m_monsterId);

	Protocol::EntityInfo* pEntityInfo = pMonsterInfo->mutable_monster_info();
	*pEntityInfo = GetGameEntityInfo();
}

void GameMonster::ApplyGameMonsterInfo(const Protocol::MonsterInfo& srcMonsterInfo)
{
	m_monsterId = srcMonsterInfo.monster_id();
	SetGameEntityInfo(srcMonsterInfo.monster_info());
}

void GameMonster::CopyGameMonsterInfo(Protocol::MonsterInfo* pDestMonsterInfo)
{
	pDestMonsterInfo->set_monster_id(m_monsterId);
	*(pDestMonsterInfo->mutable_monster_info()) = GetGameEntityInfo();
}