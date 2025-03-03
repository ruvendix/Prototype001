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

RxSendBufferPtr RxServerPacketHandler::MakeEnterGamePacket()
{
	Protocol::S_EnterGame packet;
	packet.set_entersuccess(true);
	packet.set_userid(0);

	return MakeSendBuffer(packet, Protocol::EProtocolId::EnterGame);
}