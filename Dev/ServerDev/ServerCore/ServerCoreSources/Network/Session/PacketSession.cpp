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
	if (numOfBytes < sizeof(RxPacketHeader))
	{
		return numOfBytes; // 망한 경우
	}

	RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(buffer[0]));
	if (packetHeader.packetSize > numOfBytes) // 파싱 불가능한 경우
	{
		return numOfBytes;
	}

	// 헤더 포함 크기가 패킷의 전체 크기
	ProcessReceivePacket(&buffer[0], numOfBytes);

	return numOfBytes;
}