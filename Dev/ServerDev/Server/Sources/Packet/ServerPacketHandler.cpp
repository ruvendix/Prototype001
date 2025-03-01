// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerPacketHandler.h"

#include "ServerCore/Sources/Network/Buffer/BufferWriter.h"
#include "ServerCore/Sources/Network/Buffer/BufferReader.h"

RxServerPacketHandler* RxServerPacketHandler::s_pInstance = nullptr;

RxServerPacketHandler::RxServerPacketHandler()
{

}

RxServerPacketHandler::~RxServerPacketHandler()
{

}

RxServerPacketHandler* RxServerPacketHandler::I()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new RxServerPacketHandler;
	}

	return s_pInstance;
}

void RxServerPacketHandler::Destory()
{
	SAFE_DELETE(s_pInstance);
}

void RxServerPacketHandler::HandlePacket(BYTE* buffer, int32 numOfBytes)
{
	RxBufferReader bufferReader(buffer, numOfBytes, 0);

	RxPacketHeader packetHeader;
	bufferReader.Read(&packetHeader);

	// 테스트
	if (packetHeader.protocolId == 1)
	{
		printf("서버 프로토콜(%d)\n", packetHeader.protocolId);
	}
}

RxSendBufferPtr RxServerPacketHandler::MakeTestPacket(uint64 id, uint32 hp, uint16 attack, const std::vector<BuffData>& vecBuff)
{
	Protocol::S_TEST packet;
	packet.set_id(10);
	packet.set_hp(100);
	packet.set_attack(10);

	Protocol::BuffData* pBuffData = packet.add_buffs();
	pBuffData->set_buffid(100);
	pBuffData->set_remaintime(1.2f);
	pBuffData->add_victims(10);

	pBuffData = packet.add_buffs();
	pBuffData->set_buffid(200);
	pBuffData->set_remaintime(2.2f);

	return MakeSendBuffer(packet, 1);
}