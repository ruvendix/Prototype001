// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameSession.h"

#include "GameSessionManager.h"
#include "Packet/ServerPacketHandler.h"

RxGameSession::RxGameSession()
{
	printf("RxGameSession 생성! (%p)\n", this);
}

RxGameSession::~RxGameSession()
{
	printf("RxGameSession 소멸! (%p)\n", this);
}

void RxGameSession::ProcessConnectImpl()
{
	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->AddGameSession(spGameSession);

	// 클라이언트 입장!
	Send(RxServerPacketHandler::I()->MakeEnterGamePacket());

	// 클라이언트를 방에 입장!
	GameRoom::I()->EnterGameRoom(spGameSession);
}

void RxGameSession::ProcessDisconnectImpl()
{
	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->RemoveGameSession(spGameSession);

	// 클라이언트를 방에서 내보냄!
	GameRoom::I()->LeaveGameRoom(spGameSession);
}

void RxGameSession::ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes)
{
	RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(buffer));
	printf("ProcessReceivePacket packetHeader(Id: %d, Size: %d)\n", packetHeader.protocolId, packetHeader.packetFullSize);
}

void RxGameSession::ProcessSendImpl(uint32 numOfBytes)
{
	printf("ProcessSendImpl numOfBytes(%d)\n", numOfBytes);
}