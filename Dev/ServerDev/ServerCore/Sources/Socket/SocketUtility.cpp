// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SocketUtility.h"

#include "Network/IocpEvent.h"
#include "Network/Session/Session.h"

namespace
{
	LPFN_CONNECTEX    s_connectExFn = nullptr;
	LPFN_DISCONNECTEX s_disconnectExFn = nullptr;
	LPFN_ACCEPTEX     s_acceptExFn = nullptr;
}

void RxSocketUtility::Startup()
{
	// WSA => Window Socket Application Interface
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		HandleLastError();
		return;
	}

	// 소켓 생성
	SOCKET dummySocket = CreateAsynchronousSocket(IPPROTO_TCP);

	// 원하는 함수를 런타임에 등록하는 부분
	BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&s_connectExFn));
	BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&s_disconnectExFn));
	BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&s_acceptExFn));

	CloseSocket(dummySocket);
}

void RxSocketUtility::Cleanup()
{
	::WSACleanup();
}

int32 RxSocketUtility::HandleLastError()
{
	int32 lastErrorCode = ::WSAGetLastError();

	switch (lastErrorCode)
	{
	case WSA_INVALID_HANDLE:
		printf("유효하지 않은 핸들\n");
		break;

	case WSA_OPERATION_ABORTED:
		printf("작업 중단\n");
		break;

	case WSA_IO_PENDING:
		break;

	default:
		void* msgBuffer = nullptr;
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, lastErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&msgBuffer), 0, nullptr);

		//::OutputDebugString(reinterpret_cast<LPTSTR>(msgBuffer));
		LocalFree(msgBuffer);

		break;
	}
	
	return lastErrorCode;
}

SOCKET RxSocketUtility::CreateBlockingSocket(int32 protocol)
{
	return (::socket(AF_INET, SOCK_STREAM, protocol));
}

// 논블로킹 소켓은 블로킹 소켓에서 바꿔주는 역할
SOCKET RxSocketUtility::CreateNonBlockingSocket(int32 protocol)
{
	SOCKET socket = CreateBlockingSocket(protocol);

	u_long socketMode = 1; // 논블로킹, 0이면 블로킹
	if (::ioctlsocket(socket, FIONBIO, &socketMode) == INVALID_SOCKET)
	{
		HandleLastError();
	}

	return socket;
}

SOCKET RxSocketUtility::CreateAsynchronousSocket(int32 protocol)
{
	return (::WSASocket(AF_INET, SOCK_STREAM, protocol, nullptr, 0, WSA_FLAG_OVERLAPPED));
}

void RxSocketUtility::BindWindowsFunction(SOCKET socket, GUID wsaGuid, LPVOID* ppFn)
{
	DWORD retBytes = 0;

	// SIO_GET_EXTENSION_FUNCTION_POINTER는 함수 포인터를 알려주므로 외부 함수로 받을 때는 더블 포인터를 사용!
	if (::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &wsaGuid, sizeof(wsaGuid), ppFn, sizeof(*ppFn), &retBytes, nullptr, nullptr) == SOCKET_ERROR)
	{
		HandleLastError();
	}
}

void RxSocketUtility::BindSocket(SOCKET listenSocket, const SOCKADDR_IN& netAddressData)
{
	if (::bind(listenSocket, reinterpret_cast<const SOCKADDR*>(&netAddressData), sizeof(netAddressData)) == SOCKET_ERROR)
	{
		HandleLastError();
	}
}

void RxSocketUtility::BindAnyAddress(SOCKET listenSocket, uint16 port)
{
	SOCKADDR_IN netAddressData;
	netAddressData.sin_family = AF_INET;
	netAddressData.sin_addr.s_addr = ::htonl(INADDR_ANY);
	netAddressData.sin_port = ::htons(port);

	BindSocket(listenSocket, netAddressData);
}

void RxSocketUtility::Listen(SOCKET listenSocket, int32 backlog)
{
	if (::listen(listenSocket, backlog) == SOCKET_ERROR)
	{
		HandleLastError();
	}
}

SOCKET RxSocketUtility::Accept(SOCKET listenSocket, SOCKADDR_IN* pClientAddressData)
{
	int32 addressLength = sizeof(SOCKADDR_IN);
	return (::accept(listenSocket, reinterpret_cast<SOCKADDR*>(pClientAddressData), &addressLength));
}

BOOL RxSocketUtility::AcceptEx(SOCKET listenSocket, RxSessionPtr spSession, DWORD* pReceivedBytes, RxIocpEvent* pAcceptEvent)
{
	BOOL bRet = s_acceptExFn(listenSocket,
		spSession->GetSocket(),
		spSession->GetReceiveBufferWritePosition(),
		0,
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		pReceivedBytes,
		pAcceptEvent->GetOverlapped());

	return bRet;
}

int32 RxSocketUtility::Connect(SOCKET clientSocket, const SOCKADDR_IN& netAddressData)
{
	return (::connect(clientSocket, reinterpret_cast<const SOCKADDR*>(&netAddressData), sizeof(netAddressData)));
}

BOOL RxSocketUtility::ConnectEx(SOCKET clientSocket, const SOCKADDR_IN& netAddressData, DWORD* pReceivedBytes, RxIocpEvent* pAcceptEvent)
{
	BOOL bRet = s_connectExFn(clientSocket,
		reinterpret_cast<const SOCKADDR*>(&netAddressData),
		sizeof(netAddressData),
		nullptr, 0, pReceivedBytes, pAcceptEvent->GetOverlapped());

	return bRet;
}

BOOL RxSocketUtility::DisconnectEx(SOCKET clientSocket, RxIocpEvent* pDisconnectEvent)
{
	BOOL bRet = s_disconnectExFn(clientSocket, pDisconnectEvent->GetOverlapped(), TF_REUSE_SOCKET, 0);
	return bRet;
}

int32 RxSocketUtility::Send(SOCKET socket, char* sendBuffer, int32 sendBufferSize)
{
	return (::send(socket, sendBuffer, sendBufferSize, 0));
}

int32 RxSocketUtility::Receive(SOCKET socket, char* recvBuffer, int32 recvBufferSize)
{
	return (::recv(socket, recvBuffer, recvBufferSize, 0));
}

int32 RxSocketUtility::Select(fd_set* pReadFds, fd_set* pWrites, fd_set* pExcepts)
{
	return (::select(0, pReadFds, pWrites, pExcepts, nullptr)); // 준비될 때까지 무한 대기
}

void RxSocketUtility::CloseSocket(SOCKET& inoutSocket)
{
	if (inoutSocket != INVALID_SOCKET)
	{
		::closesocket(inoutSocket);
	}

	inoutSocket = INVALID_SOCKET;
}

bool RxSocketUtility::ModifyLinger(SOCKET socket, uint16 onOff, uint16 linger)
{
	LINGER lingerData;
	lingerData.l_onoff = onOff;
	lingerData.l_linger = linger;
	
	return (ModifySocketOption(socket, SOL_SOCKET, SO_LINGER, lingerData));
}

bool RxSocketUtility::ModifyReuseAddress(SOCKET socket, bool bFlag)
{
	return (ModifySocketOption(socket, SOL_SOCKET, SO_REUSEADDR, bFlag));
}

bool RxSocketUtility::ModifySendBufferSize(SOCKET socket, int32 newSize)
{
	return (ModifySocketOption(socket, SOL_SOCKET, SO_SNDBUF, newSize));
}

bool RxSocketUtility::ModifyReceiveBufferSize(SOCKET socket, int32 newSize)
{
	return (ModifySocketOption(socket, SOL_SOCKET, SO_RCVBUF, newSize));
}

bool RxSocketUtility::ModifyTcpNoDelay(SOCKET socket, bool bFlag)
{
	return (ModifySocketOption(socket, SOL_SOCKET, TCP_NODELAY, bFlag));
}

bool RxSocketUtility::ModifyUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return (ModifySocketOption(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket));
}