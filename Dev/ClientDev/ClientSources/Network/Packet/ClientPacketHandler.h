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

	RxSendBufferPtr MakeModifyEntityLookAtDirectionPacket(const ProtocolEntityInfoPtr& spEntityInfo);
	RxSendBufferPtr MakeMoveEntityPacket(const ProtocolEntityInfoPtr& spEntityInfo);
	RxSendBufferPtr MakeModifyEntityStatePacket(const ProtocolEntityInfoPtr& spEntityInfo);
	RxSendBufferPtr MakeAttckToEntityPacket(const ProtocolEntityInfoPtr& spAttackerInfo, const ProtocolEntityInfoPtr& spVictimInfo);
	RxSendBufferPtr MakeCreateProjectilePacket(const Position2d& spawnCellPos, const Vector2d& vMoveDir, int32 projectileId);
	RxSendBufferPtr MakeDieEntityPacket(const ProtocolEntityInfoPtr& spEntityInfo);

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
	void HandlePacket_EnterGamePacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_LeaveGamePacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_CreateMainPlayerPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_SyncEntitiesPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_ModifyPlayerInformationPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_ModifyEntityLookAtDirectionPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_MoveEntityPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_ModifyEntityStatePacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_HitDamageToEntityPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_DieEntityPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_CreateProjectilePacket(BYTE* buffer, int32 numOfBytes);

private:
	std::unordered_map<uint32, PacketHandleDelegator> m_mapPacketHandler;
};