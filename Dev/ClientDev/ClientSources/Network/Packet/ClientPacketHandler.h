// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using PacketHandleDelegator = Delegator<void(BYTE* /*buffer*/, int32 /*numOfBytes*/)>;

class ClientPacketHandler : public ICoreLoop
{
	DECLARE_SINGLETON(ClientPacketHandler)

public:
	virtual void Startup() override;
	virtual void Cleanup() override;

public:
	void HandlePacket(BYTE* buffer, int32 numOfBytes);

	RxSendBufferPtr MakeSyncGamePlayerMovePacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGamePlayerInfo);
	RxSendBufferPtr MakeSyncGameEntityLookAtDirectionPacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGameEntityInfo);

private:
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
	void HandlePacket_EnterGame(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_LeaveGame(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_CreateLocalGamePlayer(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_SyncGameEntities(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_SyncGamePlayer(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_SyncGameEntityLookAtDirection(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_SyncGamePlayerMove(BYTE* buffer, int32 numOfBytes);

private:
	std::unordered_map<uint32, PacketHandleDelegator> m_mapPacketHandler;
};