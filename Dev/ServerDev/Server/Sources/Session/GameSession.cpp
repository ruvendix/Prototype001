// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameSession.h"

#include "GameSessionManager.h"

RxGameSession::RxGameSession()
{
	printf("RxGameSession »ý¼º!\n");
}

RxGameSession::~RxGameSession()
{
	printf("RxGameSession ¼Ò¸ê!\n");
}

void RxGameSession::ProcessConnectImpl()
{
	const RxGameSessionPtr& refSpGameSession = std::dynamic_pointer_cast<RxGameSession>(shared_from_this());
	RxGameSessionManager::I()->AddGameSession(refSpGameSession);
}

void RxGameSession::ProcessDisconnectImpl()
{
	const RxGameSessionPtr& refSpGameSession = std::dynamic_pointer_cast<RxGameSession>(shared_from_this());
	RxGameSessionManager::I()->RemoveGameSession(refSpGameSession);
}

void RxGameSession::ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes)
{
	RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(buffer));
	printf("ProcessReceivePacket packetHeader(Id: %d, Size: %d)\n", packetHeader.protocolId, packetHeader.packetSize);
}

void RxGameSession::ProcessSendImpl(uint32 numOfBytes)
{
	printf("ProcessSendImpl numOfBytes(%d)\n", numOfBytes);
}