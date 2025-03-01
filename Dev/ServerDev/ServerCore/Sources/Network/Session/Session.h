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

	// 컨텐츠 코드에서 재정의 필요
	virtual void   ProcessConnectImpl() { }
	virtual void   ProcessDisconnectImpl() { }
	virtual uint32 ProcessReceiveImpl(BYTE* buffer, uint32 numOfBytes) { return numOfBytes; }
	virtual void   ProcessSendImpl(uint32 numOfBytes) { }

	// 가장 큰 업무 단위
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
	
	std::mutex m_mutex; // 여러 쓰레드들이 하나의 세션을 처리할 수 있음
	std::atomic<bool> m_bAtomicConnected = false;

	SOCKET m_socket = INVALID_SOCKET;
	RxNetworkAddress m_netAddr;

	// 수신 버퍼
	RxReceiveBuffer m_receiveBuffer;

	// 송신 버퍼 (멀티쓰레드 사용)
	std::queue<RxSendBufferPtr> m_queueSendBuffer;
	std::atomic<bool> m_bAtomicSendRegistered = false;

	// 재사용하는 IocpEvent
	RxIocpEvent* m_pConnectEvent = nullptr;
	RxIocpEvent* m_pDisconnectEvent = nullptr;
	RxIocpEvent* m_pReceiveEvent = nullptr;
	RxIocpEvent* m_pSendEvent = nullptr;
};