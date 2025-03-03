// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxServerSession : public RxSession
{
public:
	RxServerSession();
	virtual ~RxServerSession();

	virtual void   ProcessConnectImpl() override;
	virtual void   ProcessDisconnectImpl() override;
	virtual uint32 ProcessReceiveImpl(BYTE* buffer, uint32 numOfBytes) override;
	virtual void   ProcessSendImpl(uint32 numOfBytes) override;
};