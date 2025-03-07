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
	// �۽��� ���۴� ���� Ŭ���̾�Ʈ�� ������ ����� �־�� ��!

	bool bRegisterSend = false;

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queueSendBuffer.push(spSendBuffer);
		bRegisterSend = (m_bAtomicSendRegistered.exchange(true) == false);

		// ���� ���� �ڵ�
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
	exchagne()�� ���� ������ �����ϴµ�
	ȣ��Ǳ� ���� ���� ��ȯ�ϴ� �Լ���

	��, exchange(false)�� ��Ȳ������
	ȣ���� true => ȣ���� false, ��ȯ�� true
	ȣ���� false => ȣ���� false, ��ȯ�� false

	�ϳ��� �����尡 atomic�� ���� �ٲٰ� ���ʷ� ������ ó���ߴٸ�
	�ٸ� �����尡 atomic�� ���� �ٲ㵵 ���� �����ǹǷ�
	�ߺ� ó���Ǹ� ���ϴ� ������ ����� �� ����
	*/
	if (m_bAtomicConnected.exchange(false) == false)
	{
		return;
	}

	std::wstring wstrTempReason = wstrReason.data();
	wstrTempReason += L" (���� ���� Ŭ���̾�Ʈ ������ �ߴ�)\n";
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

	// Ŀ��Ʈ �̺�Ʈ�� ������ ���� �̿� (���ʴ� weak_ptr�̹Ƿ� �ܺο��� ���۷��� ī��Ʈ�� ������ �Ҹ��)
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
	wsaBuffer.len = static_cast<ULONG>(m_receiveBuffer.GetRemainSize()); // ��ŭ ���� ����?

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

	// �۽��� �����͸� SendEvent�� ���
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		//uint32 writeSize = 0;
		while (m_queueSendBuffer.empty() == false)
		{
			const RxSendBufferPtr& spSendBuffer = m_queueSendBuffer.front();
			//writeSize += spSendBuffer->GetWriteSize(); // ����

			m_queueSendBuffer.pop();
			vecSendBuffer.push_back(spSendBuffer);
		}
	}

	// Scatter-Gather (����� �ִ� �����͵��� ��Ƽ� �� �濡 ������)
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

	// ���� ���
	const RxSessionPtr& spThisSession = std::dynamic_pointer_cast<RxSession>(shared_from_this());
	m_spOwner.lock()->AddSession(spThisSession);

	ProcessConnectImpl();

	// ���� ���
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
		Disconnect(L"���� ���� �����Ͱ� ����");
		return;
	}

	if (m_receiveBuffer.ProcessWrite(numOfBytes) == false)
	{
		Disconnect(L"���� ���� �����Ͱ� �ʹ� ���Ƽ� �� �� ����");
		return;
	}

	uint32 dataSize = m_receiveBuffer.GetDataSize();
	uint32 processedDataSize = ProcessReceiveImpl(m_receiveBuffer.GetReadPosition(), dataSize);

	if ((processedDataSize < 0) || // ó���� ������ ũ�Ⱑ 0���� ������ ��������
		(processedDataSize < dataSize)) // ��û�Ѹ�ŭ ó���� �������� ��������
	{
		Disconnect(L"��û�Ѹ�ŭ ���� ó���� ������");
		return;
	}

	// ���� ���� ����
	m_receiveBuffer.Cleanup();

	// ���� ���
	RegisterReceive();
}

void RxSession::ProcessSend(uint32 numOfBytes)
{
	m_pSendEvent->SetOwner(nullptr);
	m_pSendEvent->GetSendBuffer().clear();

	if (numOfBytes == 0)
	{
		Disconnect(L"�۽��� �����Ͱ� ����");
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
			// ������� ó���ϴµ��� �ٸ� �����尡 �۽� ť�� �о���� ���
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
		Disconnect(L"ġ������ ����");
		break;
	}

	return errorCode;
}

void RxSession::ReleaseConnect()
{
	SAFE_DELETE(m_pConnectEvent);
}