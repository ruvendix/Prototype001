// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxServerPacketHandler
{
	DECLARE_SINGLETON(RxServerPacketHandler)

public:
	void HandlePacket(BYTE* buffer, int32 numOfBytes);

	RxSendBufferPtr MakeEnterGamePacket();
	RxSendBufferPtr MakeCreateLocalGamePlayerPacket(const Protocol::GameEntityInfo& localGamePlayerInfo);
	RxSendBufferPtr MakeSyncGameEntitiesPacket(const Protocol::S_SyncGameEntities& syncGameEntities);

	template <typename TPacket>
	RxSendBufferPtr MakeSendBuffer(const TPacket& packet, Protocol::EProtocolId protocolId)
	{
		uint16 packetDataSize = static_cast<uint16>(packet.ByteSizeLong());
		uint16 packetFullSize = sizeof(RxPacketHeader) + packetDataSize;

		RxSendBufferPtr spSendBuffer = std::make_shared<RxSendBuffer>(packetFullSize);
		
		RxPacketHeader* pPacketHeader = reinterpret_cast<RxPacketHeader*>(spSendBuffer->GetBufferPointer());
		pPacketHeader->packetFullSize = packetFullSize;
		pPacketHeader->protocolId = protocolId;
		
		assert(packet.SerializeToArray(&pPacketHeader[1], packetDataSize));
		spSendBuffer->SetWriteSize(packetFullSize);
		return spSendBuffer;
	}

private:
	std::unordered_map<uint32, PacketHandleDelegator> m_mapPacketHandler;
};