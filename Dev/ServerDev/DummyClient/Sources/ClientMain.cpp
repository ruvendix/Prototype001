// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Session/ServerSession.h"
#include "Packet/ClientPacketHandler.h"

/*
<클라이언트의 TCP 소켓>
-socket:  소켓 생성
-connect: 서버에 접속 시도

-send: 요청한 데이터를 송신 버퍼에 복사
-recv: 수신 버퍼에 도착한 데이터를 User 수준의 버퍼에 복사
*/

RxServicePtr g_spClientService;
RxThreadPool g_threadPool; // 풀은 여러개 가능

BOOL WINAPI OnClose_ConsoleHandler(DWORD signal)
{
	if ((signal == CTRL_C_EVENT) ||
		(signal == CTRL_CLOSE_EVENT) ||
		(signal == CTRL_LOGOFF_EVENT) ||
		(signal == CTRL_SHUTDOWN_EVENT))
	{
		RxClientPacketHandler::I()->Destory();

		g_spClientService->GetIocpCorePtr()->Cleanup();
		g_threadPool.Cleanup();
		RxSocketUtility::Cleanup();

		::OutputDebugStringA("서버 프로그램 종료 (서브 쓰레드들 정리중)\n");

		// 모든 서브 쓰레드들이 안전하게 종료될 때까지 시간 벌이
		std::this_thread::sleep_for(1s);
		return TRUE;
	}
	return FALSE;
}

int main()
{
	if (!SetConsoleCtrlHandler(OnClose_ConsoleHandler, TRUE))
	{
		return EXIT_FAILURE;
	}

	RxSocketUtility::Startup();

	RxServiceInfo serviceInfo =
	{
		EServiceType::Client,
		RxNetworkAddress(L"127.0.0.1", 7777),
		std::make_shared<RxIocpCore>(),
		[]() { return std::make_shared<RxServerSession>(); },
		5
	};

	g_spClientService = std::make_shared<RxClientService>(serviceInfo);
	g_spClientService->Startup();

	for (uint32 i = 0; i < 5; ++i)
	{
		g_threadPool.AddTask(
			[&]()
			{
				bool bDrive = true;
				while (bDrive == true)
				{
					bDrive = g_spClientService->GetIocpCorePtr()->Dispatch(INFINITE);
				}
			}
		);
	}

	g_threadPool.Join();

	/*
	멀티쓰레드 콘솔 프로그램은 종료 신호를 따로 보내야함...
	종료 처리는 ConsoleHandler에서 담당
	*/

	return 0;
}