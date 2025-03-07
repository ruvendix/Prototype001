// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "NetworkAddress.h"

RxNetworkAddress::RxNetworkAddress()
{
	::ZeroMemory(&m_sockAddr, sizeof(SOCKADDR_IN));
}

RxNetworkAddress::RxNetworkAddress(SOCKADDR_IN sockAddr)
{
	m_sockAddr = sockAddr;
}

RxNetworkAddress::RxNetworkAddress(const std::wstring& wstrIp, uint16 port) :
	RxNetworkAddress()
{
	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = ::htons(port);

	IN_ADDR inAddr;
	ConvertIpToAddress(wstrIp, inAddr);
	m_sockAddr.sin_addr = inAddr;
}

void RxNetworkAddress::ConvertIpToAddress(const std::wstring& wstrIp, IN_ADDR& outAddr)
{
	::InetPtonW(AF_INET, wstrIp.c_str(), &outAddr);
}

void RxNetworkAddress::BringIpAddress(std::wstring& outWstrAddr) const
{
	WCHAR wszBuffer[DEFAULT_STRING_SIZE];
	::InetNtopW(AF_INET, &(m_sockAddr.sin_addr), wszBuffer, ARRAYSIZE(wszBuffer));
	outWstrAddr = wszBuffer;
}