// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Session.h"

#include "Network/IocpEvent.h"
#include "Network/Service.h"
#include "Network/Buffer/SendBuffer.h"

namespace
{
	const uint32 MAX_BLOCK_SIZE = 0x10000; // 64KB
}

RxSession::RxSession() :
	m_receiveBuffer(MAX_BLOCK_SIZE)
{
	m_socket = RxSocketUtility::CreateAsynchronousSocket(IPPROTO_TCP);

	m_pConnectEvent = new RxIocpEvent(ENetworkEventType::Connect);
	m_pDisconnectEvent = new RxIocpEvent(ENetworkEventType::Disconnect);
	m_pReceiveEvent = new RxIocpEvent(ENetworkEventType::Receive);
	m_pSendEvent = new RxIocpEvent(ENetworkEventType::Send);
}

RxSession::~RxSession()
{
	SAFE_DELETE(m_pConnectEvent);
	SAFE_DELETE(m_pDisconnectEvent);
	SAFE_DELETE(m_pReceiveEvent);
	SAFE_DELETE(m_pSendEvent);

	RxSocketUtility::CloseSocket(m_socket);
}

HANDLE RxSession::BringHandle()
{
	return reinterpret_cast<HANDLE>(m_socket);
}

void RxSession::Dispatch(RxIocpEvent* pIocpEvent, uint32 numOfBytes)
{
	switch (pIocpEvent->GetNetworkEvent())
	{
	case ENetworkEventType::Connect:
		ProcessConnect();
		break;

	case ENetworkEventType::Disconnect:
		ProcessDisconnect();
		break;

	case ENetworkEventType::Receive:
		ProcessReceive(numOfBytes);
		break;

	case ENetworkEventType::Send:
		ProcessSend(numOfBytes);
		break;
	}
}

void RxSession::Send(const RxSendBufferPtr& spSendBuffer)
{
	// 송신할 버퍼는 여러 클라이언트로 보내는 기능이 있어야 함!

	bool bRegisterSend = false;

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queueSendBuffer.push(spSendBuffer);
		bRegisterSend = (m_bAtomicSendRegistered.exchange(true) == false);

		// 위와 같은 코드
		/*if (_sendRegistered == false)
        {
        	_sendRegistered = true;
        	RegisterSend();
        }*/
	}

	if (bRegisterSend == true)
	{
		RegisterSend();
	}	
}

bool RxSession::Connect()
{
	return RegisterConnect();
}

void RxSession::Disconnect(const std::wstring& wstrReason)
{
	/*
	exchagne()는 넣은 값으로 변경하는데
	호출되기 전의 값을 반환하는 함수임

	즉, exchange(false)의 상황에서는
	호출전 true => 호출후 false, 반환값 true
	호출전 false => 호출후 false, 반환값 false

	하나의 쓰레드가 atomic의 값을 바꾸고 최초로 로직을 처리했다면
	다른 쓰레드가 atomic의 값을 바꿔도 값은 유지되므로
	중복 처리되면 망하는 로직을 방어할 수 있음
	*/
	if (m_bAtomicConnected.exchange(false) == false)
	{
		return;
	}

	std::wstring wstrTempReason = wstrReason.data();
	wstrTempReason += L" (접속 중인 클라이언트 강제로 중단)\n";
	wprintf_s(wstrTempReason.c_str());

	if (m_spOwner.expired() == true)
	{
		return;
	}

	const RxSessionPtr& spThisSession = std::dynamic_pointer_cast<RxSession>(shared_from_this());
	m_spOwner.lock()->ReleaseSession(spThisSession);

	RegisterDisconnect();
}

bool RxSession::RegisterConnect()
{
	if (IsConnected())
	{
		return false;
	}

	if (m_spOwner.expired() == true)
	{
		return false;
	}
	const std::shared_ptr<RxService>& spOwner = m_spOwner.lock();

	if (spOwner->GetServiceType() != EServiceType::Client)
	{
		return false;
	}

	if (RxSocketUtility::ModifyReuseAddress(m_socket, true) == false)
	{
		return false;
	}

	RxSocketUtility::BindAnyAddress(m_socket, 0);

	// 커넥트 이벤트는 세션을 따로 이용 (오너는 weak_ptr이므로 외부에서 레퍼런스 카운트를 날리면 소멸됨)
	const RxSessionPtr& spSession = std::dynamic_pointer_cast<RxSession>(shared_from_this());
	m_pConnectEvent->SetSession(spSession);
	m_pConnectEvent->SetOwner(spSession);

	DWORD dwNumOfBytes = 0;
	SOCKADDR_IN sockAddr = spOwner->GetNetworkAddress().GetSockAddr();

	if (RxSocketUtility::ConnectEx(m_socket, sockAddr, &dwNumOfBytes, m_pConnectEvent) == FALSE)
	{
		int32 errorCode = RxSocketUtility::HandleLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_pConnectEvent->SetSession(nullptr);
			return false;
		}
	}

	return true;
}

bool RxSession::RegisterDisconnect()
{
	m_pDisconnectEvent->SetOwner(SharedFromThisExactType<RxIocpObject>());

	if (RxSocketUtility::DisconnectEx(m_socket, m_pDisconnectEvent) == FALSE)
	{
		int32 errorCode = RxSocketUtility::HandleLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_pDisconnectEvent->SetOwner(nullptr);
			return false;
		}
	}

	return true;
}

void RxSession::RegisterReceive()
{
	if (IsConnected() == false)
	{
		return;
	}

	m_pReceiveEvent->SetOwner(SharedFromThisExactType<RxIocpObject>());

	WSABUF wsaBuffer;
	wsaBuffer.buf = reinterpret_cast<char*>(m_receiveBuffer.GetWrtiePosition());
	wsaBuffer.len = static_cast<ULONG>(m_receiveBuffer.GetRemainSize()); // 얼만큼 읽을 건지?

	DWORD flags = 0;
	DWORD dwNumOfBytes = 0;

	if (::WSARecv(m_socket, &wsaBuffer, 1, &dwNumOfBytes, &flags, m_pReceiveEvent->GetOverlapped(), nullptr) == SOCKET_ERROR)
	{
		int32 errorCode = HandleLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_pReceiveEvent->SetOwner(nullptr);
		}
	}
}

void RxSession::RegisterSend()
{
	if (IsConnected() == false)
	{
		return;
	}
	
	m_pSendEvent->SetOwner(SharedFromThisExactType<RxIocpObject>());

	std::vector<RxSendBufferPtr>& vecSendBuffer = m_pSendEvent->GetSendBuffer();

	// 송신할 데이터를 SendEvent에 등록
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		//uint32 writeSize = 0;
		while (m_queueSendBuffer.empty() == false)
		{
			const RxSendBufferPtr& spSendBuffer = m_queueSendBuffer.front();
			//writeSize += spSendBuffer->GetWriteSize(); // 누적

			m_queueSendBuffer.pop();
			vecSendBuffer.push_back(spSendBuffer);
		}
	}

	// Scatter-Gather (흩어져 있는 데이터들을 모아서 한 방에 보낸다)
	std::vector<WSABUF> vecWsaBuffer;
	vecWsaBuffer.reserve(vecSendBuffer.size());
	for (const RxSendBufferPtr& spSendBuffer : vecSendBuffer)
	{
		assert(spSendBuffer != nullptr);

		WSABUF wsaBuffer;
		wsaBuffer.buf = reinterpret_cast<char*>(spSendBuffer->GetBufferPointer());
		wsaBuffer.len = static_cast<ULONG>(spSendBuffer->GetWriteSize());
		vecWsaBuffer.push_back(wsaBuffer);
	}

	DWORD dwNumOfBytes = 0;
	if (::WSASend(m_socket, vecWsaBuffer.data(), static_cast<DWORD>(vecWsaBuffer.size()),
			&dwNumOfBytes, 0, m_pSendEvent->GetOverlapped(), nullptr) == SOCKET_ERROR)
	{
		int32 errorCode = HandleLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			m_pSendEvent->SetOwner(nullptr);
			m_pSendEvent->GetSendBuffer().clear();
			m_bAtomicSendRegistered = false;
		}
	}
}

void RxSession::ProcessConnect()
{
	if (m_spOwner.expired() == true)
	{
		return;
	}

	m_pConnectEvent->SetOwner(nullptr);
	m_bAtomicConnected = true;

	// 세션 등록
	const RxSessionPtr& spThisSession = std::dynamic_pointer_cast<RxSession>(shared_from_this());
	m_spOwner.lock()->AddSession(spThisSession);

	ProcessConnectImpl();

	// 수신 등록
	RegisterReceive();
}

void RxSession::ProcessDisconnect()
{
	m_pDisconnectEvent->SetOwner(nullptr);
	ProcessDisconnectImpl();
}

void RxSession::ProcessReceive(uint32 numOfBytes)
{
	m_pReceiveEvent->SetOwner(nullptr);

	if (numOfBytes == 0)
	{
		Disconnect(L"수신 받은 데이터가 없음");
		return;
	}

	if (m_receiveBuffer.ProcessWrite(numOfBytes) == false)
	{
		Disconnect(L"수신 받은 데이터가 너무 많아서 쓸 수 없음");
		return;
	}

	uint32 dataSize = m_receiveBuffer.GetDataSize();
	uint32 processedDataSize = ProcessReceiveImpl(m_receiveBuffer.GetReadPosition(), dataSize);

	if ((processedDataSize < 0) || // 처리된 데이터 크기가 0보다 작으면 문제있음
		(processedDataSize < dataSize)) // 요청한만큼 처리를 못했으니 문제있음
	{
		Disconnect(L"요청한만큼 수신 처리를 못했음");
		return;
	}

	// 수신 버퍼 정리
	m_receiveBuffer.Cleanup();

	// 수신 등록
	RegisterReceive();
}

void RxSession::ProcessSend(uint32 numOfBytes)
{
	m_pSendEvent->SetOwner(nullptr);
	m_pSendEvent->GetSendBuffer().clear();

	if (numOfBytes == 0)
	{
		Disconnect(L"송신할 데이터가 없음");
		return;
	}

	ProcessSendImpl(numOfBytes);

	bool bRegisteredSend = false;
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_queueSendBuffer.empty() == true)
		{
			m_bAtomicSendRegistered = false;
		}
		else
		{
			// 여기까지 처리하는동안 다른 쓰레드가 송신 큐에 밀어넣은 경우
			bRegisteredSend = true;
		}
	}

	if (bRegisteredSend == true)
	{
		RegisterSend();
	}
}

int32 RxSession::HandleLastError()
{
	int32 errorCode = RxSocketUtility::HandleLastError();

	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"치명적인 오류");
		break;
	}

	return errorCode;
}

void RxSession::ReleaseConnect()
{
	SAFE_DELETE(m_pConnectEvent);
}