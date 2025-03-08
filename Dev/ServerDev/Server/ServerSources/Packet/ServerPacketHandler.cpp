// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerPacketHandler.h"

DEFINE_SINGLETON(RxServerPacketHandler);

void RxServerPacketHandler::HandlePacket(BYTE* buffer, int32 numOfBytes)
{
	RxBufferReader bufferReader(buffer, numOfBytes, 0);

	RxPacketHeader packetHeader;
	bufferReader.Read(&packetHeader);

	// 구조상 쉽게
	auto foundIter = m_mapPacketHandler.find(packetHeader.protocolId);
	if (foundIter == m_mapPacketHandler.cend())
	{
		return;
	}

	foundIter->second.InvokeFunctions(buffer, numOfBytes);
}

RxSendBufferPtr RxServerPacketHandler::MakeEnterGamePacket()
{
	Protocol::S_EnterGame enterGamePacket;
	enterGamePacket.set_enter_success(true);
	enterGamePacket.set_user_id(0);

	return MakeSendBuffer(enterGamePacket, Protocol::EProtocolId::EnterGame);
}

RxSendBufferPtr RxServerPacketHandler::MakeCreateLocalGamePlayerPacket(const Protocol::GameEntityInfo& localGamePlayerInfo)
{
	Protocol::S_CreateLocalGamePlayer localGamePlayerPacket;
	Protocol::GameEntityInfo* pLocalGamePlayerInfo = localGamePlayerPacket.mutable_local_game_player_info();
	*pLocalGamePlayerInfo = localGamePlayerInfo;

	return MakeSendBuffer(localGamePlayerPacket, Protocol::EProtocolId::CreateLocalGamePlayer);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGameEntitiesPacket(const Protocol::S_SyncGameEntities& syncGameEntities)
{
	return MakeSendBuffer(syncGameEntities, Protocol::EProtocolId::SyncGameEntities);
}