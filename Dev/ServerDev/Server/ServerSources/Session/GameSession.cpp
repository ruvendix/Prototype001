// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameSession.h"

#include "GameSessionManager.h"
#include "Packet/ServerPacketHandler.h"

RxGameSession::RxGameSession()
{
	printf("RxGameSession ����! (%p)\n", this);
}

RxGameSession::~RxGameSession()
{
	printf("RxGameSession �Ҹ�! (%p)\n", this);
}

void RxGameSession::ProcessConnectImpl()
{
	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->AddGameSession(spGameSession);

	// Ŭ���̾�Ʈ ����!
	Send(RxServerPacketHandler::I()->MakeEnterGamePacket());

	// Ŭ���̾�Ʈ�� �濡 ����!
	GameRoom::I()->EnterGameRoom(spGameSession);
}

void RxGameSession::ProcessDisconnectImpl()
{
	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->RemoveGameSession(spGameSession);

	// Ŭ���̾�Ʈ�� �濡�� ������!
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