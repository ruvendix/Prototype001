// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerSession.h"

namespace
{
	char g_arrDataBuffer[] = "Hello world";
}

RxServerSession::RxServerSession()
{
	printf("RxServerSession »ý¼º!\n");
}

RxServerSession::~RxServerSession()
{
	printf("RxServerSession ¼Ò¸ê!\n");
}

void RxServerSession::ProcessConnectImpl()
{
	printf("Connected to server\n");

	//RxSendBufferPtr spSendBuffer = std::make_shared<RxSendBuffer>(4096);
	//spSendBuffer->CopyBuffer(g_arrDataBuffer, sizeof(g_arrDataBuffer));

	//Send(spSendBuffer);
}

void RxServerSession::ProcessDisconnectImpl()
{
	printf("Disconnected from server\n");
}

uint32 RxServerSession::ProcessReceiveImpl(BYTE* buffer, uint32 numOfBytes)
{
	printf("ProcessReceiveImpl numOfBytes(%d)\n", numOfBytes);

	std::this_thread::sleep_for(0.1s);

	ClientPacketHandler::I()->HandlePacket(buffer, numOfBytes);

	//RxSendBufferPtr spSendBuffer = std::make_shared<RxSendBuffer>(4096);
	//spSendBuffer->CopyBuffer(g_arrDataBuffer, sizeof(g_arrDataBuffer));

	//Send(spSendBuffer);
	return numOfBytes;
}

void RxServerSession::ProcessSendImpl(uint32 numOfBytes)
{
	printf("ProcessSendImpl numOfBytes(%d)\n", numOfBytes);
}