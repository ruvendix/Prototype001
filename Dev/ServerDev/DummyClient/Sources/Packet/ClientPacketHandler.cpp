// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientPacketHandler.h"

#include "ServerCore/Sources/Network/Buffer/BufferReader.h"

// �̷� �� Common
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

	// ���� ������
	// 1) ���ڿ� (ex. name)
	// 2) �׳� ����Ʈ �迭 (ex. ��� �̹���)
	// 3) �Ϲ� ����Ʈ
	std::vector<BuffData> vecBuff;
};

RxClientPacketHandler* RxClientPacketHandler::s_pInstance = nullptr;

RxClientPacketHandler::RxClientPacketHandler()
{

}

RxClientPacketHandler::~RxClientPacketHandler()
{

}

RxClientPacketHandler* RxClientPacketHandler::I()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new RxClientPacketHandler;
	}

	return s_pInstance;
}

void RxClientPacketHandler::Destory()
{
	SAFE_DELETE(s_pInstance);
}

void RxClientPacketHandler::HandlePacket(BYTE* buffer, int32 numOfBytes)
{
	RxBufferReader bufferReader(buffer, numOfBytes, 0);

	RxPacketHeader packetHeader;
	bufferReader.Read(&packetHeader);

	// �׽�Ʈ
	if (packetHeader.protocolId == 1)
	{
		printf("Ŭ���̾�Ʈ ��������(%d)\n", packetHeader.protocolId);
		PasingTestPacket(buffer, numOfBytes);
	}
}

void RxClientPacketHandler::PasingTestPacket(BYTE* buffer, int32 numOfBytes)
{
	RxPacketHeader* pPacketHeader = reinterpret_cast<RxPacketHeader*>(buffer);
	uint16 packetSize = pPacketHeader->packetSize;
	uint16 protocolId = pPacketHeader->protocolId;

	Protocol::S_TEST packet;
	packet.ParseFromArray(&pPacketHeader[1], packetSize - sizeof(RxPacketHeader));

	uint64 id = packet.id();
	uint32 hp = packet.hp();
	uint32 attack = packet.attack();
	printf("(Id: %llu, Hp: %d, Attack: %d)\n", id, hp, attack);

	printf("Buff Count: %d\n", packet.buffs_size());
	for (int32 i = 0; i < packet.buffs_size(); ++i)
	{
		const Protocol::BuffData& buffData = packet.buffs(i);
		printf("BufInfo: (Id: %llu, remainTime: %f)\n", buffData.buffid(), buffData.remaintime());
	}
}