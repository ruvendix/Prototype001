// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerSession.h"

#include "GameApplication/GameApplication.h"

RxServerSession::RxServerSession()
{
	::OutputDebugString("���� ���� ����!\n");
}

RxServerSession::~RxServerSession()
{
	::OutputDebugString("���� ���� �Ҹ�!\n");
}

void RxServerSession::ProcessConnectImpl()
{
	DEFAULT_TRACE_LOG("Connected to server\n");
}

void RxServerSession::ProcessConnectFailedImpl()
{
	DETAIL_ERROR_LOG(EErrorCode::ConnectFailedToServer);
	GameApplication::I()->SetForceShutdown(true);
}

void RxServerSession::ProcessDisconnectImpl()
{
	DETAIL_ERROR_LOG(EErrorCode::DisconnectFromServer);
	GameApplication::I()->SetForceShutdown(true);
}

void RxServerSession::ProcessSendImpl(uint32 numOfBytes)
{
	DEFAULT_TRACE_LOG("numOfBytes(%d)\n", numOfBytes);
}

void RxServerSession::ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes)
{
	RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(buffer));
	DEFAULT_TRACE_LOG("ProcessReceivePacket packetHeader(Id: %d, Size: %d)", packetHeader.protocolId, packetHeader.packetFullSize);
	ClientPacketHandler::I()->HandlePacket(buffer, numOfBytes);
}