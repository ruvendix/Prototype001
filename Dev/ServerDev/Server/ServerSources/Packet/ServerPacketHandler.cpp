// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerPacketHandler.h"

DEFINE_SINGLETON(RxServerPacketHandler);

void RxServerPacketHandler::Startup()
{
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGamePlayerMove, &RxServerPacketHandler::HandlePacket_SyncGamePlayerMove);
}

void RxServerPacketHandler::Cleanup()
{

}

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

RxSendBufferPtr RxServerPacketHandler::MakeSyncGamePlayerPacket(const Protocol::GameEntityInfo& syncGamePlayerInfo)
{
	Protocol::S_SyncGamePlayer syncGamePlayer;
	Protocol::GameEntityInfo* pGamePlayerInfo = syncGamePlayer.mutable_game_player_info();
	(*pGamePlayerInfo) = syncGamePlayerInfo;

	return MakeSendBuffer(syncGamePlayer, Protocol::EProtocolId::SyncGamePlayer);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGamePlayerMovePacket(const Protocol::GameEntityInfo& syncGamePlayerInfo)
{
	Protocol::S_SyncGamePlayerMove syncGamePlayerMove;
	Protocol::GameEntityInfo* pGamePlayerInfo = syncGamePlayerMove.mutable_game_player_info();
	(*pGamePlayerInfo) = syncGamePlayerInfo;

	return MakeSendBuffer(syncGamePlayerMove, Protocol::EProtocolId::SyncGamePlayerMove);
}

void RxServerPacketHandler::HandlePacket_SyncGamePlayerMove(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_SyncGamePlayerMove, packet);
	GameRoom::I()->ParsingPacket_SyncGamePlayerMove(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 이동 패킷 받음!");
}