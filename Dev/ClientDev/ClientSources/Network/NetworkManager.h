// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using RxServerSessionPtr = std::shared_ptr<class RxServerSession>;

class NetworkManager : public ICoreLoop
{
	DECLARE_SINGLETON(NetworkManager)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	RxServerSessionPtr CreateServerSession();
	void SendPacket(const RxSendBufferPtr& spPacket);

private:
	RxServicePtr m_spClientService = nullptr;
	RxServerSessionPtr m_spServerSession = nullptr;
};