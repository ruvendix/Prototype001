// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class ENetworkEventType : uint8
{
	Unknown = 0,
	Connect,
	Disconnect,
	Accept,
	Receive,
	Send,
};

class RxSession;

// OVERLAPPED ������ ����Ʈ �����ͷ� ����� �� ����
class RxIocpEvent
{
public:
	RxIocpEvent(ENetworkEventType netEvent);
	RxIocpEvent(const RxIocpObjectPtr& spOwner, ENetworkEventType netEvent);
	~RxIocpEvent() = default;

	ENetworkEventType GetNetworkEvent() const { return m_netEvent; }

	OVERLAPPED* GetOverlapped() { return &m_overlapped; }
	const RxSessionPtr& GetSession() const { return m_spSession; }
	void SetSession(const RxSessionPtr& spSession) { m_spSession = spSession; }
	
	RxIocpObjectPtr GetOwner() const { return (m_spOwner.lock()); }
	void SetOwner(const RxIocpObjectPtr& spOwner) { m_spOwner = spOwner; }

	std::vector<RxSendBufferPtr>& GetSendBuffer() { return m_vecSendBuffer; }

private:
	OVERLAPPED m_overlapped; // ������ ó���� �־� ��!
	std::weak_ptr<RxIocpObject> m_spOwner;

	ENetworkEventType m_netEvent = ENetworkEventType::Unknown;
	RxSessionPtr m_spSession = nullptr;

	// ������ �̺�Ʈ���� Ŭ������ ��������� �̰� �ϳ��� �޶� �ǹ̰� ����
	std::vector<RxSendBufferPtr> m_vecSendBuffer;
};