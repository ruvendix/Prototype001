// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class ClientPacketHandler
{
	DECLARE_SINGLETON(ClientPacketHandler)

public:
	void HandlePacket(BYTE* buffer, int32 numOfBytes);
	void PasingTestPacket(BYTE* buffer, int32 numOfBytes);
};