// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameSessionManager.h"

#include "GameSession.h"

RxGameSessionManager* RxGameSessionManager::s_pInstance = nullptr;

RxGameSessionManager* RxGameSessionManager::I()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new RxGameSessionManager;
	}

	return s_pInstance;
}

void RxGameSessionManager::Destory()
{
	SAFE_DELETE(s_pInstance);
}

RxGameSessionManager::RxGameSessionManager()
{

}

RxGameSessionManager::~RxGameSessionManager()
{

}

void RxGameSessionManager::AddGameSession(const RxGameSessionPtr& spGameSession)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_setSession.insert(spGameSession);
}

void RxGameSessionManager::RemoveGameSession(const RxGameSessionPtr& spGameSession)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_setSession.erase(spGameSession);
}

void RxGameSessionManager::Broadcast(const RxSendBufferPtr& spSendBuffer)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	for (RxGameSessionPtr spGameSession : m_setSession)
	{
		spGameSession->Send(spSendBuffer);
	}
}