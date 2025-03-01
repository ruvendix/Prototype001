// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxGameSessionManager
{
public:
	static RxGameSessionManager* I();
	static void Destory();

public:
	RxGameSessionManager();
	~RxGameSessionManager();

	void AddGameSession(const RxGameSessionPtr& spGameSession);
	void RemoveGameSession(const RxGameSessionPtr& spGameSession);
	void Broadcast(const RxSendBufferPtr& spSendBuffer);

private:
	static RxGameSessionManager* s_pInstance;

	std::mutex m_mutex;
	std::set<RxGameSessionPtr> m_setSession;
};