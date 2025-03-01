// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// ���� �Ǵ� Ŭ���̾�Ʈ�� ó���ϴ� ���� ū �۾� ����

enum class EServiceType
{
	Unknown = -1,
	Client,
	Server
};

using SessionFactoryFunc = std::function<RxSessionPtr(void)>;

struct RxServiceInfo
{
	EServiceType serviceType = EServiceType::Unknown;
	RxNetworkAddress netAddress;
	RxIocpCorePtr spIocpCore = nullptr;
	SessionFactoryFunc sessionFactoryFunc = nullptr;
	uint32 maxSessionCount = 1;
};

class RxService : public std::enable_shared_from_this<RxService>
{
public:
	RxService(const RxServiceInfo& serviceInfo);
	virtual ~RxService();

	virtual bool Startup() abstract;
	virtual void Cleanup() abstract;

	RxSessionPtr CreateSession(); // ������ ����ϴ� �� ����
	void AddSession(RxSessionPtr spSession);
	void ReleaseSession(const RxSessionPtr& spSession);

	bool IsExistSessionFactory() const { return (m_serviceInfo.sessionFactoryFunc != nullptr); }

	uint32 GetMaxSessionCount() const { return m_serviceInfo.maxSessionCount; }
	const RxNetworkAddress& GetNetworkAddress() const { return m_serviceInfo.netAddress; }
	const RxIocpCorePtr& GetIocpCorePtr() const { return m_serviceInfo.spIocpCore; }
	EServiceType GetServiceType() const { return m_serviceInfo.serviceType; }

private:
	std::mutex m_mutex;
	RxServiceInfo m_serviceInfo;

	// ������ �ߺ��Ǹ� ���ϹǷ�
	std::set<RxSessionPtr> m_setSession;
};
/////////////////////////////////////////////////////////////////
class RxClientService : public RxService
{
	using Super = RxService;

public:
	using Super::Super;
	virtual ~RxClientService();

	virtual bool Startup() override;
	virtual void Cleanup() override;

private:

};
/////////////////////////////////////////////////////////////////
class RxServerService : public RxService
{
	using Super = RxService;

public:
	using Super::Super;
	virtual ~RxServerService();

	virtual bool Startup() override;
	virtual void Cleanup() override;

private:
	RxListenerPtr m_spListener = nullptr;
};