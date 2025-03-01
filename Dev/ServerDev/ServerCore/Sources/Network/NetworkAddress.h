// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RxNetworkAddress
{
public:
	RxNetworkAddress();
	RxNetworkAddress(SOCKADDR_IN sockAddr);
	RxNetworkAddress(const std::wstring& wstrIp, uint16 port);

	void ConvertIpToAddress(const std::wstring& wstrIp, IN_ADDR& outAddr);
	void BringIpAddress(std::wstring& outWstrAddr) const;

	const SOCKADDR_IN& GetSockAddr() const { return m_sockAddr; }
	uint16 GetPort() const { return (::ntohs(m_sockAddr.sin_port)); }

private:
	SOCKADDR_IN m_sockAddr;
};