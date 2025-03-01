// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

struct BuffData
{
	uint64 buffId;
	float remainTime;
};

class RxServerPacketHandler
{
public:
	static RxServerPacketHandler* I();
	static void Destory();

public:
	RxServerPacketHandler();
	~RxServerPacketHandler();

	void HandlePacket(BYTE* buffer, int32 numOfBytes);
	RxSendBufferPtr MakeTestPacket(uint64 id, uint32 hp, uint16 attack, const std::vector<BuffData>& vecBuff);

	template <typename TPacket>
	RxSendBufferPtr MakeSendBuffer(const TPacket& packet, uint16 protocolId)
	{
		uint16 dataSize = static_cast<uint16>(packet.ByteSizeLong());
		uint16 packetSize = sizeof(RxPacketHeader) + dataSize;

		RxSendBufferPtr spSendBuffer = std::make_shared<RxSendBuffer>(packetSize);
		
		RxPacketHeader* pPacketHeader = reinterpret_cast<RxPacketHeader*>(spSendBuffer->GetBufferPointer());
		pPacketHeader->packetSize = packetSize;
		pPacketHeader->protocolId = protocolId;
		
		assert(packet.SerializeToArray(&pPacketHeader[1], dataSize));
		spSendBuffer->SetWriteSize(packetSize);
		return spSendBuffer;
	}

private:
	static RxServerPacketHandler* s_pInstance;
};