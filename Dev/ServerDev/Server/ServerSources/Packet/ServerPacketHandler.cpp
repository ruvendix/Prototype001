// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerPacketHandler.h"

DEFINE_SINGLETON(RxServerPacketHandler);

void RxServerPacketHandler::Startup()
{
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::MoveEntity, &RxServerPacketHandler::HandlePacket_MoveEntityPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::ModifyEntityLookAtDir, &RxServerPacketHandler::HandlePacket_ModifyEntityLookAtDirectionPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::ModifyEntityState, &RxServerPacketHandler::HandlePacket_ModifyEntityStatePacket);
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
	Protocol::S_EnterGamePacket enterGamePacket;
	enterGamePacket.set_enter_success(true);
	enterGamePacket.set_user_id(gameSessionId);

	return MakeSendBuffer(enterGamePacket, Protocol::EProtocolId::EnterGame);
}

RxSendBufferPtr RxServerPacketHandler::MakeLeaveGamePacket(uint64 gameSessionId, const std::shared_ptr<GamePlayer>& spLeaveGamePlayer)
{
	Protocol::S_LeaveGamePacket leaveGamePacket;
	leaveGamePacket.set_leave_success(true);
	leaveGamePacket.set_user_id(gameSessionId);

	Protocol::NetworkEntityInfo* pPlayerInfo = leaveGamePacket.mutable_player_info();
	(*pPlayerInfo) = spLeaveGamePlayer->GetGameEntityInfo();

	return MakeSendBuffer(leaveGamePacket, Protocol::EProtocolId::LeaveGame);
}

RxSendBufferPtr RxServerPacketHandler::MakeCreateMainPlayerPacket(const Protocol::NetworkEntityInfo& mainPlayerInfo)
{
	Protocol::S_CreateMainPlayerPacket createMainPlayerPacket;
	Protocol::NetworkEntityInfo* pMainPlayerInfo = createMainPlayerPacket.mutable_main_player_info();
	(*pMainPlayerInfo) = mainPlayerInfo;

	return MakeSendBuffer(createMainPlayerPacket, Protocol::EProtocolId::CreateMainPlayer);
}

RxSendBufferPtr RxServerPacketHandler::MakeSyncEntitiesPacket(const Protocol::S_SyncEntitiesPacket& syncEntitiesPacket)
{
	return MakeSendBuffer(syncEntitiesPacket, Protocol::EProtocolId::SyncEntities);
}

RxSendBufferPtr RxServerPacketHandler::MakeModifyPlayerInformationPacket(const Protocol::NetworkEntityInfo& playerInfo)
{
	Protocol::S_ModifyPlayerInformationPacket modifyPlayerInfoPacket;
	Protocol::NetworkEntityInfo* pPlayerInfo = modifyPlayerInfoPacket.mutable_player_info();
	(*pPlayerInfo) = playerInfo;

	return MakeSendBuffer(modifyPlayerInfoPacket, Protocol::EProtocolId::ModifyPlayerInformation);
}

RxSendBufferPtr RxServerPacketHandler::MakeModifyEntityLookAtDirectionPacket(const Protocol::NetworkEntityInfo& entityInfo)
{
	Protocol::S_ModifyEntityLookAtDirectionPacket modifyEntityLookAtDirectionPacket;
	Protocol::NetworkEntityInfo* pEntityInfo = modifyEntityLookAtDirectionPacket.mutable_entity_info();
	(*pEntityInfo) = entityInfo;

	return MakeSendBuffer(modifyEntityLookAtDirectionPacket, Protocol::EProtocolId::ModifyEntityLookAtDir);
}

RxSendBufferPtr RxServerPacketHandler::MakeMoveEntityPacket(const Protocol::NetworkEntityInfo& entityInfo)
{
	Protocol::S_MoveEntityPacket moveEntityPacket;
	Protocol::NetworkEntityInfo* pGameEntityInfo = moveEntityPacket.mutable_entity_info();
	(*pGameEntityInfo) = entityInfo;

	return MakeSendBuffer(moveEntityPacket, Protocol::EProtocolId::MoveEntity);
}

RxSendBufferPtr RxServerPacketHandler::MakeModifyEntityStatePacket(const Protocol::NetworkEntityInfo& entityInfo)
{
	Protocol::S_ModifyEntityStatePacket modifyEntityStatePacket;
	Protocol::NetworkEntityInfo* pGameEntityInfo = modifyEntityStatePacket.mutable_entity_info();
	(*pGameEntityInfo) = entityInfo;

	return MakeSendBuffer(modifyEntityStatePacket, Protocol::EProtocolId::ModifyEntityState);
}

RxSendBufferPtr RxServerPacketHandler::MakeHitDamageToEntityPacket(const Protocol::NetworkEntityInfo& attackerEntityInfo, const Protocol::NetworkEntityInfo& victimEntityInfo)
{
	Protocol::S_HitDamageToEntityPacket hitDamageToEntityPacket;

	Protocol::NetworkEntityInfo* pAttackerInfo = hitDamageToEntityPacket.mutable_attacker_info();
	(*pAttackerInfo) = attackerEntityInfo;

	Protocol::NetworkEntityInfo* pVictimInfo = hitDamageToEntityPacket.mutable_victim_info();
	(*pVictimInfo) = victimEntityInfo;

	return MakeSendBuffer(hitDamageToEntityPacket, Protocol::EProtocolId::HitDamageToEntity);
}

void RxServerPacketHandler::HandlePacket_ModifyEntityLookAtDirectionPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_ModifyEntityLookAtDirectionPacket, packet);
	GameRoom::I()->ParsingPacket_ModifyEntityLookAtDirectionPacket(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 게임 엔티티의 보는 방향 받음!");
}

void RxServerPacketHandler::HandlePacket_MoveEntityPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_MoveEntityPacket, packet);
	GameRoom::I()->ParsingPacket_MoveEntityPacket(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 이동 패킷 받음!");
}

void RxServerPacketHandler::HandlePacket_ModifyEntityStatePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::C_ModifyEntityStatePacket, packet);
	GameRoom::I()->ParsingPacket_ModifyEntityStatePacket(packet);
	DEFAULT_TRACE_LOG("클라이언트로부터 상태 패킷 받음!");
}