// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerPacketHandler.h"

DEFINE_SINGLETON(RxServerPacketHandler);

void RxServerPacketHandler::Startup()
{
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityLookAtDir, &RxServerPacketHandler::HandlePacket_SyncGameEntityLookAtDirectionPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGamePlayerMove, &RxServerPacketHandler::HandlePacket_SyncGamePlayerMovePacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityState, &RxServerPacketHandler::HandlePacket_SyncGameEntityStatePacket);
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

RxSendBufferPtr RxServerPacketHandler::MakeEnterGamePacket(uint64 gameSessionId)
{
	Protocol::S_EnterGame enterGamePacket;
	enterGamePacket.set_enter_success(true);
	enterGamePacket.set_user_id(gameSessionId);

	return MakeSendBuffer(enterGamePacket, Protocol::EProtocolId::EnterGame);
}

RxSendBufferPtr RxServerPacketHandler::MakeLeaveGamePacket(uint64 gameSessionId, const std::shared_ptr<GamePlayer>& spLeaveGamePlayer)
{
	Protocol::S_LeaveGame leaveGamePacket;
	leaveGamePacket.set_leave_success(true);
	leaveGamePacket.set_user_id(gameSessionId);

	Protocol::GameEntityInfo* pGamePlayerInfo = leaveGamePacket.mutable_game_player_info();
	(*pGamePlayerInfo) = spLeaveGamePlayer->GetGameEntityInfo();

	return MakeSendBuffer(leaveGamePacket, Protocol::EProtocolId::LeaveGame);
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

RxSendBufferPtr RxServerPacketHandler::MakeSyncGameEntityLookAtDirectionPacket(const Protocol::GameEntityInfo& syncGameEntityInfo)
{
	Protocol::S_SyncGamePlayerMove syncGameEntityLookAtDir;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityLookAtDir.mutable_game_player_info();
	(*pGameEntityInfo) = syncGameEntityInfo;

	return MakeSendBuffer(syncGameEntityLookAtDir, Protocol::EProtocolId::SyncGameEntityLookAtDir);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGamePlayerMovePacket(const Protocol::GameEntityInfo& syncGamePlayerInfo)
{
	Protocol::S_SyncGamePlayerMove syncGamePlayerMove;
	Protocol::GameEntityInfo* pGamePlayerInfo = syncGamePlayerMove.mutable_game_player_info();
	(*pGamePlayerInfo) = syncGamePlayerInfo;

	return MakeSendBuffer(syncGamePlayerMove, Protocol::EProtocolId::SyncGamePlayerMove);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGameEntityStatePacket(const Protocol::GameEntityInfo& syncGameEntityInfo)
{
	Protocol::S_SyncGameEntityState syncGameEntityState;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityState.mutable_game_player_info();
	(*pGameEntityInfo) = syncGameEntityInfo;

	return MakeSendBuffer(syncGameEntityState, Protocol::EProtocolId::SyncGameEntityState);
}

void RxServerPacketHandler::HandlePacket_SyncGameEntityLookAtDirectionPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_SyncGameEntityLookAtDir, packet);
	GameRoom::I()->ParsingPacket_SyncGameEntityLookAtDirection(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 게임 엔티티의 보는 방향 받음!");
}

void RxServerPacketHandler::HandlePacket_SyncGamePlayerMovePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_SyncGamePlayerMove, packet);
	GameRoom::I()->ParsingPacket_SyncGamePlayerMove(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 이동 패킷 받음!");
}

void RxServerPacketHandler::HandlePacket_SyncGameEntityStatePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_SyncGameEntityState, packet);
	GameRoom::I()->ParsingPacket_SyncGameEntityState(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 상태 패킷 받음!");
}