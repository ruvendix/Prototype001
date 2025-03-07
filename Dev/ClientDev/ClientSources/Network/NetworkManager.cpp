// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "NetworkManager.h"

DEFINE_SINGLETON(NetworkManager);

void NetworkManager::Startup()
{
	RxSocketUtility::Startup();

	RxServiceInfo serviceInfo =
	{
		EServiceType::Client,
		RxNetworkAddress(L"127.0.0.1", 7777),
		std::make_shared<RxIocpCore>(),
		[&]() { return CreateServerSession(); },
		1
	};

	// Service´Â CommonÀÓ...
	m_spClientService = std::make_shared<RxClientService>(serviceInfo);
	m_spClientService->Startup();
}

bool NetworkManager::Update(float deltaSeconds)
{
	return m_spClientService->GetIocpCorePtr()->Dispatch(0);
}

void NetworkManager::Cleanup()
{
	m_spServerSession->ReleaseConnect();
	m_spClientService->GetIocpCorePtr()->Cleanup();
	RxSocketUtility::Cleanup();
}

RxServerSessionPtr NetworkManager::CreateServerSession()
{
	m_spServerSession = std::make_shared<RxServerSession>();
	return m_spServerSession;
}