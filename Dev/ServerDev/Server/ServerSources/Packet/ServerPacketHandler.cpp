// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerPacketHandler.h"

DEFINE_SINGLETON(RxServerPacketHandler);

void RxServerPacketHandler::Startup()
{
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityLookAtDir, &RxServerPacketHandler::HandlePacket_SyncGameEntityLookAtDirectionPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityMove, &RxServerPacketHandler::HandlePacket_SyncGameEntityMovePacket);
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

	Protocol::GameEntityInfo* pGameEntityInfo = leaveGamePacket.mutable_entity_info();
	(*pGameEntityInfo) = spLeaveGamePlayer->GetGameEntityInfo();

	return MakeSendBuffer(leaveGamePacket, Protocol::EProtocolId::LeaveGame);
}

RxSendBufferPtr RxServerPacketHandler::MakeCreateLocalGamePlayerPacket(const Protocol::GameEntityInfo& syncGameEntityInfo)
{
	Protocol::S_CreateLocalGamePlayer localGamePlayerPacket;
	Protocol::GameEntityInfo* pGameEntityInfo = localGamePlayerPacket.mutable_entity_info();
	(*pGameEntityInfo) = syncGameEntityInfo;

	return MakeSendBuffer(localGamePlayerPacket, Protocol::EProtocolId::CreateLocalGamePlayer);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGameEntitiesPacket(const Protocol::S_SyncGameEntities& syncGameEntities)
{
	return MakeSendBuffer(syncGameEntities, Protocol::EProtocolId::SyncGameEntities);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGamePlayerPacket(const Protocol::GameEntityInfo& syncGameEntityInfo)
{
	Protocol::S_SyncGamePlayer syncGamePlayerPacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGamePlayerPacket.mutable_entity_info();
	(*pGameEntityInfo) = syncGameEntityInfo;

	return MakeSendBuffer(syncGamePlayerPacket, Protocol::EProtocolId::SyncGamePlayer);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGameEntityLookAtDirectionPacket(const Protocol::GameEntityInfo& syncGameEntityInfo)
{
	Protocol::S_SyncGameEntityLookAtDir syncGameEntityLookAtDirPacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityLookAtDirPacket.mutable_entity_info();
	(*pGameEntityInfo) = syncGameEntityInfo;

	return MakeSendBuffer(syncGameEntityLookAtDirPacket, Protocol::EProtocolId::SyncGameEntityLookAtDir);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGameEntityMovePacket(const Protocol::GameEntityInfo& syncGameEntityInfo)
{
	Protocol::S_SyncGameEntityMove syncGameEntityMovePacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityMovePacket.mutable_entity_info();
	(*pGameEntityInfo) = syncGameEntityInfo;

	return MakeSendBuffer(syncGameEntityMovePacket, Protocol::EProtocolId::SyncGameEntityMove);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncGameEntityStatePacket(const Protocol::GameEntityInfo& syncGameEntityInfo)
{
	Protocol::S_SyncGameEntityState syncGameEntityStatePacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityStatePacket.mutable_entity_info();
	(*pGameEntityInfo) = syncGameEntityInfo;

	return MakeSendBuffer(syncGameEntityStatePacket, Protocol::EProtocolId::SyncGameEntityState);
}

RxSendBufferPtr RxServerPacketHandler::MakeAttackToGameEntityPacket(const Protocol::GameEntityInfo& attackerEntityInfo, const Protocol::GameEntityInfo& victimEntityInfo)
{
	Protocol::S_AttckToGameEntity attckToGameEntityPacket;

	Protocol::GameEntityInfo* pAttackerInfo = attckToGameEntityPacket.mutable_attacker_info();
	(*pAttackerInfo) = attackerEntityInfo;

	Protocol::GameEntityInfo* pVictimInfo = attckToGameEntityPacket.mutable_victim_info();
	(*pVictimInfo) = victimEntityInfo;

	return MakeSendBuffer(attckToGameEntityPacket, Protocol::EProtocolId::AttackToGameEntity);
}

void RxServerPacketHandler::HandlePacket_SyncGameEntityLookAtDirectionPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_SyncGameEntityLookAtDir, packet);
	GameRoom::I()->ParsingPacket_SyncGameEntityLookAtDirection(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 게임 엔티티의 보는 방향 받음!");
}

void RxServerPacketHandler::HandlePacket_SyncGameEntityMovePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_SyncGameEntityMove, packet);
	GameRoom::I()->ParsingPacket_SyncGameEntityMove(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 이동 패킷 받음!");
}

void RxServerPacketHandler::HandlePacket_SyncGameEntityStatePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_SyncGameEntityState, packet);
	GameRoom::I()->ParsingPacket_SyncGameEntityState(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 상태 패킷 받음!");
}