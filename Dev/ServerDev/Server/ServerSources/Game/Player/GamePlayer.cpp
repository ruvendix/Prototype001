// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GamePlayer.h"

GamePlayer::GamePlayer()
{

}

GamePlayer::~GamePlayer()
{

}

void GamePlayer::Startup()
{

}

void GamePlayer::Cleanup()
{

}

void GamePlayer::AddToSyncEntitiesPacket(Protocol::S_SyncEntitiesPacket& outSyncEntitiesPacket)
{
	Protocol::EntityInfo* pPlayerInfo = outSyncEntitiesPacket.add_players_info();
	*pPlayerInfo = GetGameEntityInfo();
}