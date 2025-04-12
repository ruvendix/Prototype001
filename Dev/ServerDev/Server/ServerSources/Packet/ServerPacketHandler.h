// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxServerPacketHandler : public ICoreLoop
{
	DECLARE_SINGLETON(RxServerPacketHandler)

public:
	virtual void Startup() override;
	virtual void Cleanup() override;

public:
	void HandlePacket(BYTE* buffer, int32 numOfBytes);

	RxSendBufferPtr MakeEnterGamePacket(uint64 gameSessionId);
	RxSendBufferPtr MakeLeaveGamePacket(uint64 gameSessionId, const std::shared_ptr<GamePlayer>& spLeaveGamePlayer);
	RxSendBufferPtr MakeCreateMainPlayerPacket(const Protocol::EntityInfo& mainPlayerInfo);
	RxSendBufferPtr MakeSyncEntitiesPacket(const Protocol::S_SyncEntitiesPacket& syncEntitiesPacket);
	RxSendBufferPtr MakeModifyPlayerInformationPacket(const Protocol::EntityInfo& playerInfo);
	RxSendBufferPtr MakeModifyEntityLookAtDirectionPacket(const Protocol::EntityInfo& entityInfo);
	RxSendBufferPtr MakeMoveEntityPacket(const Protocol::EntityInfo& entityInfo);
	RxSendBufferPtr MakeModifyEntityStatePacket(const Protocol::EntityInfo& entityInfo);
	RxSendBufferPtr MakeHitDamageToEntityPacket(const Protocol::EntityInfo& attackerEntityInfo, const Protocol::EntityInfo& victimEntityInfo);
	RxSendBufferPtr MakeDieEntityPacket(const Protocol::EntityInfo& victimEntityInfo);
	RxSendBufferPtr MakeCreateProjectilePacket(const Protocol::ProjectileInfo& projectileInfo);

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
	void HandlePacket_ModifyEntityLookAtDirectionPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_MoveEntityPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_ModifyEntityStatePacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_AttackToEntityPacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_CreateProjectilePacket(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_DieEntityPacket(BYTE* buffer, int32 numOfBytes);

private:
	std::unordered_map<uint32, PacketHandleDelegator> m_mapPacketHandler;
};