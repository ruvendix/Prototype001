// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxIocpEvent;

class RxSession : public RxIocpObject
{
public:
	RxSession();
	virtual ~RxSession();

	virtual HANDLE BringHandle() override;
	virtual void Dispatch(RxIocpEvent* pIocpEvent, uint32 numOfBytes) override;

	// ������ �ڵ忡�� ������ �ʿ�
	virtual void   ProcessConnectImpl() { }
	virtual void   ProcessDisconnectImpl() { }
	virtual uint32 ProcessReceiveImpl(BYTE* buffer, uint32 numOfBytes) { return numOfBytes; }
	virtual void   ProcessSendImpl(uint32 numOfBytes) { }

	// ���� ū ���� ����
	void Send(const RxSendBufferPtr& spSendBuffer);
	bool Connect();
	void Disconnect(const std::wstring& wstrReason);

	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterReceive();
	void RegisterSend();
		 
	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessReceive(uint32 numOfBytes);
	void ProcessSend(uint32 numOfBytes);

	int32 HandleLastError();

	SOCKET GetSocket() const { return m_socket; }
	BYTE* GetReceiveBufferWritePosition() { return m_receiveBuffer.GetWrtiePosition(); }
	void SetNetworkAddress(const RxNetworkAddress& netAddr) { m_netAddr = netAddr; }
	void SetOwner(const RxServicePtr& spService) { m_spOwner = spService; }

	bool IsConnected() const { return (m_bAtomicConnected == true); }

private:
	std::weak_ptr<RxService> m_spOwner;
	
	std::mutex m_mutex; // ���� ��������� �ϳ��� ������ ó���� �� ����
	std::atomic<bool> m_bAtomicConnected = false;

	SOCKET m_socket = INVALID_SOCKET;
	RxNetworkAddress m_netAddr;

	// ���� ����
	RxReceiveBuffer m_receiveBuffer;

	// �۽� ���� (��Ƽ������ ���)
	std::queue<RxSendBufferPtr> m_queueSendBuffer;
	std::atomic<bool> m_bAtomicSendRegistered = false;

	// �����ϴ� IocpEvent
	RxIocpEvent* m_pConnectEvent = nullptr;
	RxIocpEvent* m_pDisconnectEvent = nullptr;
	RxIocpEvent* m_pReceiveEvent = nullptr;
	RxIocpEvent* m_pSendEvent = nullptr;
};