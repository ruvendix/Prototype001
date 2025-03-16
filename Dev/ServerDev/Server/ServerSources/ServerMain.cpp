// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"

#include "Session/GameSession.h"
#include "Session/GameSessionManager.h"
#include "Packet/ServerPacketHandler.h"

RxServicePtr g_spServerService;
RxThreadPool g_threadPool; // Ǯ�� ������ ����

/*
���� ����
1) IOCP �ڵ� �ݱ�(���� �۾� ���� ���� ��������� �۾��� �ߴܵǾ����Ƿ� ��� ����)
2) ������Ǯ�� �ִ� ��� ���� ������鿡�� �����϶�� ��ȣ�� ����
3) ���� ���� �ݱ�
4) ��� ���� �����尡 �����ϰ� ����Ǿ�� �ϹǷ� ���� ������� 0.1�� ���
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

		DEFAULT_TRACE_LOG("���� ���α׷� ���� (���� ������� ������)");

		GameRoom::I()->Cleanup();
		LogSystem::I()->Cleanup();
		ErrorHandler::I()->Cleanup();

		// ��� ���� ��������� �����ϰ� ����� ������ �ð� ����
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
	��Ƽ������ �ܼ� ���α׷��� ���� ��ȣ�� ���� ��������...
	���� ó���� ConsoleHandler���� ���
	*/

	g_threadPool.Join();
#else
	while (true)
	{
		::timeBeginPeriod(1);
		::Sleep(16);
		g_spServerService->GetIocpCorePtr()->Dispatch(0);
		GameRoom::I()->Update(0.016f); // ������ 60���������� ����
		::timeEndPeriod(1);
	}
#endif

	return 0;
}