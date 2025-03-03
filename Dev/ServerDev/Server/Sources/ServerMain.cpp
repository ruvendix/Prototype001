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
		RxGameSessionManager::I()->Destory();
		RxServerPacketHandler::I()->Destory();

		g_spServerService->GetIocpCorePtr()->Cleanup();
		g_threadPool.Cleanup();
		RxSocketUtility::Cleanup();

		::OutputDebugStringA("���� ���α׷� ���� (���� ������� ������)\n");

		// ��� ���� ��������� �����ϰ� ����� ������ �ð� ����
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
		EServiceType::Server,
		RxNetworkAddress(L"127.0.0.1", 7777),
		std::make_shared<RxIocpCore>(),
		[]() { return std::make_shared<RxGameSession>(); },
		100
	};

	g_spServerService = std::make_shared<RxServerService>(serviceInfo);
	g_spServerService->Startup();

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

	g_threadPool.Join();

	/*
	��Ƽ������ �ܼ� ���α׷��� ���� ��ȣ�� ���� ��������...
	���� ó���� ConsoleHandler���� ���
	*/

	return 0;
}