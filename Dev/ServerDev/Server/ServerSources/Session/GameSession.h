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

public:
	void SetGamePlayer(const GamePlayerPtr& spGamePlayer) { m_spGamePlayer = spGamePlayer; }

	GamePlayerPtr GetGamePlayer() const
	{
		ASSERT_LOG_RETURN_VALUE(m_spGamePlayer.expired() == false, nullptr);
		return (m_spGamePlayer.lock());
	}

private:
	std::weak_ptr<GamePlayer> m_spGamePlayer;
};