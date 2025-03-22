// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxServerSession : public RxPacketSession
{
public:
	RxServerSession();
	virtual ~RxServerSession();

	virtual void ProcessConnectImpl() override;
	virtual void ProcessConnectFailedImpl() override;	
	virtual void ProcessDisconnectImpl() override;
	virtual void ProcessSendImpl(uint32 numOfBytes) override;

	virtual void ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes) override;
};