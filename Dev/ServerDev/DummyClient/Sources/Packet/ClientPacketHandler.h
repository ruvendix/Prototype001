// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxClientPacketHandler
{
public:
	static RxClientPacketHandler* I();
	static void Destory();

public:
	RxClientPacketHandler();
	~RxClientPacketHandler();

	void HandlePacket(BYTE* buffer, int32 numOfBytes);
	void PasingTestPacket(BYTE* buffer, int32 numOfBytes);

private:
	static RxClientPacketHandler* s_pInstance;
};