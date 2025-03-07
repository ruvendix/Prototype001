// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxGameSession : public RxPacketSession
{
public:
	RxGameSession();
	virtual ~RxGameSession();

	virtual void ProcessConnectImpl() override;
	virtual void ProcessDisconnectImpl() override;
	virtual void ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes) override;
	virtual void ProcessSendImpl(uint32 numOfBytes) override;
};