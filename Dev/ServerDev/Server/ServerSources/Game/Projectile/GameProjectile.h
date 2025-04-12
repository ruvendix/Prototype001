// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameProjectile : public GameEntity
{
public:
	GameProjectile();
	virtual ~GameProjectile();

	virtual void Startup() override;
	virtual void Cleanup() override;

public:
	void SetGameSession(const RxGameSessionPtr& spGameSession) { m_spGameSession = spGameSession; }
	const RxGameSessionPtr& GetGameSession() const { return m_spGameSession; }

private:
	RxGameSessionPtr m_spGameSession = nullptr;
};