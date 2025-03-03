// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientPacketHandler.h"

#include "ServerCore/Sources/Network/Buffer/BufferReader.h"

DEFINE_SINGLETON(ClientPacketHandler);

// 이런 건 Common
struct BuffData
{
	uint64 buffId;
	float remainTime;
};

struct TestPacket
{
	uint64 id;
	uint32 hp;
	uint16 attack;

	// 가변 데이터
	// 1) 문자열 (ex. name)
	// 2) 그냥 바이트 배열 (ex. 길드 이미지)
	// 3) 일반 리스트
	std::vector<BuffData> vecBuff;
};

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 numOfBytes)
{
	RxBufferReader bufferReader(buffer, numOfBytes, 0);

	RxPacketHeader packetHeader;
	bufferReader.Read(&packetHeader);

	// 테스트
	if (packetHeader.protocolId == 1)
	{
		DEFAULT_TRACE_LOG("클라이언트 프로토콜(%d)", packetHeader.protocolId);
		PasingTestPacket(buffer, numOfBytes);
	}
}

void ClientPacketHandler::PasingTestPacket(BYTE* buffer, int32 numOfBytes)
{
	RxPacketHeader* pPacketHeader = reinterpret_cast<RxPacketHeader*>(buffer);
	uint16 packetSize = pPacketHeader->packetSize;
	uint16 protocolId = pPacketHeader->protocolId;

	Protocol::S_TEST packet;
	packet.ParseFromArray(&pPacketHeader[1], packetSize - sizeof(RxPacketHeader));

	uint64 id = packet.id();
	uint32 hp = packet.hp();
	uint32 attack = packet.attack();
	DEFAULT_TRACE_LOG("(Id: %llu, Hp: %d, Attack: %d)", id, hp, attack);

	DEFAULT_TRACE_LOG("Buff Count: %d\n", packet.buffs_size());
	for (int32 i = 0; i < packet.buffs_size(); ++i)
	{
		const Protocol::BuffData& buffData = packet.buffs(i);
		DEFAULT_TRACE_LOG("BufInfo: (Id: %llu, remainTime: %f)", buffData.buffid(), buffData.remaintime());
	}
}