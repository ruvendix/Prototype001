// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PacketSession.h"

RxPacketSession::RxPacketSession()
{

}

RxPacketSession::~RxPacketSession()
{

}

uint32 RxPacketSession::ProcessReceiveImpl(BYTE* buffer, uint32 numOfBytes)
{
	// 여러 패킷이 뭉쳐서 올 수 있으므로 쪼갤 필요가 있음
	uint32 readSuccessSize = 0;

	while (true)
	{
		uint32 chunkSize = numOfBytes - readSuccessSize;

		// 최소한 헤더는 파싱할 수 있어야함!
		if (chunkSize < sizeof(RxPacketHeader))
		{
			break;
		}

		RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(&buffer[readSuccessSize]));
		if (chunkSize < packetHeader.packetFullSize) // 패킷 파싱이 불가능한 경우
		{
			break;
		}

		// 패킷 파싱 가능!
		ProcessReceivePacket(&buffer[readSuccessSize], packetHeader.packetFullSize);

		// 파싱에 성공하면 다음 위치로 이동
		readSuccessSize += packetHeader.packetFullSize;
	}

	return readSuccessSize;
}