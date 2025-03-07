// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(GameEntityIdCounter);

class GameRoom : public EnableSharedClass
{
	DECLARE_SINGLETON(GameRoom)

public:
	virtual void Startup() override;
	virtual void Cleanup() override;

public:
	void EnterGameSession(const RxGameSessionPtr& spGameSession);

private:
	std::unordered_map<uint32, GameEntityPtr> m_mapGamePlayer;
	std::unordered_map<uint32, GameEntityPtr> m_mapGameMonster;
};