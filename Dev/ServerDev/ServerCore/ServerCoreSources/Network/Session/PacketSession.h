// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Session.h"

struct RxPacketHeader
{
	uint16 protocolId = 0; // �̰� ���� enum���� ��
	uint16 packetSize = 0;
};

class RxPacketSession : public RxSession
{
public:
	RxPacketSession();
	virtual ~RxPacketSession();

	virtual uint32 ProcessReceiveImpl(BYTE* buffer, uint32 numOfBytes) override;
	virtual void ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes) = 0;
};