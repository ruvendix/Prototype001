// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"

#include "Session/GameSession.h"
#include "Session/GameSessionManager.h"
#include "Packet/ServerPacketHandler.h"

RxServicePtr g_spServerService;
RxThreadPool g_threadPool; // 풀은 여러개 가능

/*
종료 절차
1) IOCP 핸들 닫기(관련 작업 중인 서브 쓰레드들은 작업이 중단되었으므로 대기 상태)
2) 쓰레드풀에 있는 모든 서브 쓰레드들에게 종료하라고 신호를 보냄
3) 서버 소켓 닫기
4) 모든 서브 쓰레드가 안전하게 종료되어야 하므로 메인 쓰레드는 0.1초 대기
*/
BOOL WINAPI OnClose_ConsoleHandler(DWORD signal)
{
	if ((signal == CTRL_C_EVENT) ||
		(signal == CTRL_CLOSE_EVENT) ||
		(signal == CTRL_LOGOFF_EVENT) ||
		(signal == CTRL_SHUTDOWN_EVENT))
	{
		g_spServerService->GetIocpCorePtr()->Cleanup();
		g_spServerService->Cleanup();
		g_threadPool.Cleanup();
		RxSocketUtility::Cleanup();

		DEFAULT_TRACE_LOG("서버 프로그램 종료 (서브 쓰레드들 정리중)");

		GameRoom::I()->Cleanup();
		LogSystem::I()->Cleanup();
		ErrorHandler::I()->Cleanup();

		// 모든 서브 쓰레드들이 안전하게 종료될 때까지 시간 벌이
		//std::this_thread::sleep_for(1s);
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

	LogSystem::I()->Startup();
	LogSystem::I()->RegisterFileLoggerPathKey("ServerLog");
	FILE_LOG(LogDefault, "Test");

	ErrorHandler::I()->Startup();

	RxSocketUtility::Startup();

	RxServiceInfo serviceInfo =
	{
		EServiceType::Server,
		RxNetworkAddress(L"127.0.0.1", 7777),
		std::make_shared<RxIocpCore>(),
		[]() { return std::make_shared<RxGameSession>(); },
		100
	};

	g_spServerService = std::make_shared<RxServerService>(serviceInfo);
	g_spServerService->Startup();

	RxServerPacketHandler::I()->Startup();
	GameRoom::I()->Startup();

#if 0
	for (uint32 i = 0; i < 5; ++i)
	{
		g_threadPool.AddTask(
			[&]()
			{
				bool bDrive = true;
				while (bDrive == true)
				{
					bDrive = g_spServerService->GetIocpCorePtr()->Dispatch(INFINITE);
				}
			}
		);
	}

	/*
	멀티쓰레드 콘솔 프로그램은 종료 신호를 따로 보내야함...
	종료 처리는 ConsoleHandler에서 담당
	*/

	g_threadPool.Join();
#else
	while (true)
	{
		::timeBeginPeriod(1);
		::Sleep(16);
		g_spServerService->GetIocpCorePtr()->Dispatch(0);
		GameRoom::I()->Update(0.016f); // 서버도 60프레임으로 맞춤
		::timeEndPeriod(1);
	}
#endif

	return 0;
}