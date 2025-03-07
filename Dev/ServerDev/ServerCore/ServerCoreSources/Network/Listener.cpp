// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Listener.h"

#include "Service.h"
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"
#include "Session/Session.h"

RxListener::RxListener(const RxServicePtr& spOwner)
{
	m_spOwner = spOwner;
	m_listenSocket = RxSocketUtility::CreateAsynchronousSocket(IPPROTO_TCP);
}

RxListener::~RxListener()
{
	for (RxIocpEvent* pIocpEvent : m_acceptEvents)
	{
		SAFE_DELETE(pIocpEvent);
	}

	RxSocketUtility::CloseSocket(m_listenSocket);
}

HANDLE RxListener::BringHandle()
{
	return (reinterpret_cast<HANDLE>(m_listenSocket));
}

void RxListener::Dispatch(RxIocpEvent* pIocpEvent, uint32 numOfBytes)
{
	assert(pIocpEvent->GetNetworkEvent() == ENetworkEventType::Accept);
	ProcessAccept(pIocpEvent);
}

bool RxListener::ReadyToAccept()
{
	if (RxSocketUtility::ModifyReuseAddress(m_listenSocket, true) == false)
	{
		return false;
	}

	if (RxSocketUtility::ModifyLinger(m_listenSocket, 0, 0) == false)
	{
		return false;
	}

	if (m_spOwner.expired() == true)
	{
		return false;
	}

	const RxNetworkAddress& netAddress = m_spOwner.lock()->GetNetworkAddress();
	RxSocketUtility::BindSocket(m_listenSocket, netAddress.GetSockAddr());

	RxSocketUtility::Listen(m_listenSocket, SOMAXCONN);

	const uint32 acceptCount = 1;
	for (uint32 i = 0; i < acceptCount; ++i)
	{
		RxIocpEvent* pAcceptEvent = new RxIocpEvent(SharedFromThisExactType<RxIocpObject>(), ENetworkEventType::Accept);
		m_acceptEvents.push_back(pAcceptEvent);
		RegisterAccept(pAcceptEvent);
	}

	return true;
}

void RxListener::RegisterAccept(RxIocpEvent* pAcceptEvent)
{
	if (m_spOwner.expired() == true)
	{
		return;
	}

	RxSessionPtr spSession = m_spOwner.lock()->CreateSession();
	pAcceptEvent->SetSession(spSession);

	DWORD dwReceivedBytes = 0;
	if (RxSocketUtility::AcceptEx(m_listenSocket, spSession, &dwReceivedBytes, pAcceptEvent) == FALSE)
	{
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			// 다시 시도
			RegisterAccept(pAcceptEvent);
		}
	}
}

void RxListener::ProcessAccept(RxIocpEvent* pAcceptEvent)
{
	RxSessionPtr spSession = pAcceptEvent->GetSession();
	if (RxSocketUtility::ModifyUpdateAcceptSocket(spSession->GetSocket(), m_listenSocket) == false)
	{
		// 다시 시도
		RegisterAccept(pAcceptEvent);
		return;
	}

	SOCKADDR_IN sockAddr;
	int32 sockAddrSize = sizeof(SOCKADDR_IN);
	::ZeroMemory(&sockAddr, sockAddrSize);

	if (::getpeername(spSession->GetSocket(), reinterpret_cast<SOCKADDR*>(&sockAddr), &sockAddrSize) == SOCKET_ERROR)
	{
		// 다시 시도
		RegisterAccept(pAcceptEvent);
		return;
	}

	printf("Client connected!\n");

	RxNetworkAddress networkAddr(sockAddr);
	spSession->SetNetworkAddress(networkAddr);
	spSession->ProcessConnect();

	// 비동기 함수니까 반드시 재호출 필요!
	RegisterAccept(pAcceptEvent);
}