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
	// ���� ��Ŷ�� ���ļ� �� �� �����Ƿ� �ɰ� �ʿ䰡 ����
	uint32 readSuccessSize = 0;

	while (true)
	{
		uint32 chunkSize = numOfBytes - readSuccessSize;

		// �ּ��� ����� �Ľ��� �� �־����!
		if (chunkSize < sizeof(RxPacketHeader))
		{
			break;
		}

		RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(&buffer[readSuccessSize]));
		if (chunkSize < packetHeader.packetFullSize) // ��Ŷ �Ľ��� �Ұ����� ���
		{
			break;
		}

		// ��Ŷ �Ľ� ����!
		ProcessReceivePacket(&buffer[readSuccessSize], packetHeader.packetFullSize);

		// �Ľ̿� �����ϸ� ���� ��ġ�� �̵�
		readSuccessSize += packetHeader.packetFullSize;
	}

	return readSuccessSize;
}