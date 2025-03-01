// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "IocpCore.h"

#include "IocpObject.h"
#include "IocpEvent.h"

RxIocpCore::RxIocpCore()
{

}

RxIocpCore::~RxIocpCore()
{
	
}

void RxIocpCore::Startup()
{
	m_hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	assert(m_hIocp != INVALID_HANDLE_VALUE);
}

void RxIocpCore::Cleanup()
{
	::CloseHandle(m_hIocp);
}

bool RxIocpCore::Register(RxIocpObjectPtr spIocpObj)
{
	assert(spIocpObj != nullptr);
	return (::CreateIoCompletionPort(spIocpObj->BringHandle(), m_hIocp, 0, 0));
}

bool RxIocpCore::Dispatch(uint32 timeMilliseconds)
{
	DWORD dwNumOfBytes = 0;
	ULONG_PTR key = 0;
	RxIocpEvent* pIocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(m_hIocp, &dwNumOfBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&pIocpEvent), timeMilliseconds) == FALSE)
	{
		RxIocpObjectPtr spIocpObj = nullptr;

		switch (RxSocketUtility::HandleLastError())
		{
		case WSA_OPERATION_ABORTED: // 다시 시도			
			RxSocketUtility::HandleLastError();

			spIocpObj = pIocpEvent->GetOwner();
			spIocpObj->Dispatch(pIocpEvent, dwNumOfBytes);

			break;

		default: // 무조건 아웃
			::OutputDebugStringA("무조건 아웃임!\n");
			return false;
		}
	}
	else
	{
		RxIocpObjectPtr spIocpObj = pIocpEvent->GetOwner();
		spIocpObj->Dispatch(pIocpEvent, dwNumOfBytes);
	}

	return true;
}
