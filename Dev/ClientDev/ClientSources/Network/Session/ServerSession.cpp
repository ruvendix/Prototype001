// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerSession.h"

RxServerSession::RxServerSession()
{
	DEFAULT_TRACE_LOG("RxServerSession »ý¼º!\n");
}

RxServerSession::~RxServerSession()
{
	DEFAULT_TRACE_LOG("RxServerSession ¼Ò¸ê!\n");
}

void RxServerSession::ProcessConnectImpl()
{
	DEFAULT_TRACE_LOG("Connected to server\n");
}

void RxServerSession::ProcessDisconnectImpl()
{
	DEFAULT_TRACE_LOG("Disconnected from server\n");
}

void RxServerSession::ProcessSendImpl(uint32 numOfBytes)
{
	DEFAULT_TRACE_LOG("numOfBytes(%d)\n", numOfBytes);
}

void RxServerSession::ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes)
{
	DEFAULT_TRACE_LOG("numOfBytes(%d)", numOfBytes);
	ClientPacketHandler::I()->HandlePacket(buffer, numOfBytes);
}