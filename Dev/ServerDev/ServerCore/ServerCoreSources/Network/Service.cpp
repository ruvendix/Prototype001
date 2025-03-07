// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Service.h"

#include "IocpCore.h"
#include "Listener.h"
#include "Session/Session.h"

RxService::RxService(const RxServiceInfo& serviceInfo)
{
	m_serviceInfo = serviceInfo;
}

RxService::~RxService()
{

}

bool RxService::Startup()
{
	GetIocpCorePtr()->Startup();
	return true;
}

void RxService::Cleanup()
{

}

RxSessionPtr RxService::CreateSession()
{
	RxSessionPtr spSession = m_serviceInfo.sessionFactoryFunc();
	spSession->SetOwner(shared_from_this()); // 세션은 이 시점에 오너를 등록

	if (m_serviceInfo.spIocpCore->Register(spSession) == false)
	{
		return nullptr;
	}

	return spSession;
}

void RxService::AddSession(RxSessionPtr spSession) // 여기서 레퍼런스 카운트 증가
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_setSession.insert(spSession); // 레퍼런스 카운트 증가 안함
}

void RxService::ReleaseSession(const RxSessionPtr& spSession)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_setSession.erase(spSession);
}
/////////////////////////////////////////////////////////////////
RxClientService::~RxClientService()
{

}

bool RxClientService::Startup()
{
	if (Super::Startup() == false)
	{
		return false;
	}

	if (IsExistSessionFactory() == false)
	{
		return false;
	}

	const uint32 sessionCount = GetMaxSessionCount();
	for (uint32 i = 0; i < sessionCount; ++i)
	{
		RxSessionPtr spSession = CreateSession();
		if (spSession->Connect() == false)
		{
			return false;
		}
	}

	return true;
}

void RxClientService::Cleanup()
{
	Super::Cleanup();
}
/////////////////////////////////////////////////////////////////
RxServerService::~RxServerService()
{

}

bool RxServerService::Startup()
{
	if (Super::Startup() == false)
	{
		return false;
	}

	if (IsExistSessionFactory() == false)
	{
		return false;
	}

	m_spListener = std::make_shared<RxListener>(shared_from_this());
	assert(m_spListener != nullptr);

	if (GetIocpCorePtr()->Register(m_spListener) == false)
	{
		return false;
	}

	if (m_spListener->ReadyToAccept() == false)
	{
		return false;
	}

	return true;
}

void RxServerService::Cleanup()
{
	Super::Cleanup();
}