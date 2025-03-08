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

private:
	void HandlePacket_EnterGame(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_CreateLocalGamePlayer(BYTE* buffer, int32 numOfBytes);
	void HandlePacket_SyncGameEntities(BYTE* buffer, int32 numOfBytes);

private:
	std::unordered_map<uint32, PacketHandleDelegator> m_mapPacketHandler;
};