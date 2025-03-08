// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#define REGISTER_PACKET_HANDLER(protocolId, packetHandler) \
{ \
	PacketHandleDelegator packetHandleDelegator; \
	packetHandleDelegator.ConnectFunction(this, packetHandler); \
	auto insertedIter = m_mapPacketHandler.insert(std::make_pair(TO_NUM(protocolId), packetHandleDelegator)); \
	if (insertedIter.second == false) \
	{ \
		DEFAULT_TRACE_LOG("이미 등록된 프로토콜 Id(%d)", TO_NUM(protocolId)); \
	} \
}

#define START_PACKET_CONTENTS(rawBuffer, PacketType, identifier) \
	RxPacketHeader* pPacketHeader = reinterpret_cast<RxPacketHeader*>(rawBuffer); \
	PacketType identifier; \
	identifier.ParseFromArray(&pPacketHeader[1], (pPacketHeader->packetFullSize) - sizeof(RxPacketHeader));