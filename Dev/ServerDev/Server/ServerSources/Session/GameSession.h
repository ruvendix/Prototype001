// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(UserIdCounter);

class RxGameSession : public RxPacketSession
{
public:
	RxGameSession();
	virtual ~RxGameSession();

	virtual void ProcessConnectImpl() override;
	virtual void ProcessDisconnectImpl() override;
	virtual void ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes) override;
	virtual void ProcessSendImpl(uint32 numOfBytes) override;

public:
	void SetNetworkPlayer(const GamePlayerPtr& spPlayer) { m_spGamePlayer = spPlayer; }

	GamePlayerPtr GetGamePlayer() const
	{
		ASSERT_LOG_RETURN_VALUE(m_spGamePlayer.expired() == false, nullptr);
		return (m_spGamePlayer.lock());
	}

private:
	uint64 m_userId = 0;
	std::weak_ptr<GamePlayer> m_spGamePlayer;
};