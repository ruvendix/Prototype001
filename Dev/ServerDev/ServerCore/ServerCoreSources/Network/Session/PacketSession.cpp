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
		return numOfBytes; // ���� ���
	}

	RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(buffer[0]));
	if (packetHeader.packetSize > numOfBytes) // �Ľ� �Ұ����� ���
	{
		return numOfBytes;
	}

	// ��� ���� ũ�Ⱑ ��Ŷ�� ��ü ũ��
	ProcessReceivePacket(&buffer[0], numOfBytes);

	return numOfBytes;
}