// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Session/ServerSession.h"
#include "Packet/ClientPacketHandler.h"

/*
<Ŭ���̾�Ʈ�� TCP ����>
-socket:  ���� ����
-connect: ������ ���� �õ�

-send: ��û�� �����͸� �۽� ���ۿ� ����
-recv: ���� ���ۿ� ������ �����͸� User ������ ���ۿ� ����
*/

RxServicePtr g_spClientService;
RxThreadPool g_threadPool; // Ǯ�� ������ ����

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
	��Ƽ������ �ܼ� ���α׷��� ���� ��ȣ�� ���� ��������...
	���� ó���� ConsoleHandler���� ���
	*/

	return 0;
}