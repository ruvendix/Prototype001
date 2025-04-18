// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GamePlayer : public GameEntity
{
public:
	GamePlayer();
	virtual ~GamePlayer();

	virtual void Startup() override;
	virtual void Cleanup() override;

	virtual void AddToSyncEntitiesPacket(Protocol::S_SyncEntitiesPacket& outSyncEntitiesPacket) override;

public:
	void SetGameSession(const RxGameSessionPtr& spGameSession) { m_spGameSession = spGameSession; }
	const RxGameSessionPtr& GetGameSession() const { return m_spGameSession; }

private:
	RxGameSessionPtr m_spGameSession = nullptr;
};