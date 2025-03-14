// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameSession.h"

#include "GameSessionManager.h"
#include "Packet/ServerPacketHandler.h"

RxGameSession::RxGameSession()
{
	DEFAULT_TRACE_LOG("(%p 게임 세션) 생성!\n", this);
}

RxGameSession::~RxGameSession()
{
	DEFAULT_TRACE_LOG("(%p 게임 세션) 소멸!", this);
}

void RxGameSession::ProcessConnectImpl()
{
	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->AddGameSession(spGameSession);

	// 클라이언트 입장!
	Send(RxServerPacketHandler::I()->MakeEnterGamePacket());

	// 클라이언트를 방에 입장!
	GameRoom::I()->EnterGameRoom(spGameSession);
	DEFAULT_TRACE_LOG("(%p 게임 세션) 입장", spGameSession);
}

void RxGameSession::ProcessDisconnectImpl()
{
	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->RemoveGameSession(spGameSession);

	// 클라이언트를 방에서 내보냄!
	GameRoom::I()->LeaveGameRoom(spGameSession);
	DEFAULT_TRACE_LOG("(%p 게임 세션) 나감", spGameSession);
}

void RxGameSession::ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes)
{
	RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(buffer));
	DEFAULT_TRACE_LOG("받은 패킷 헤더(Id: %d, Size: %d)", packetHeader.protocolId, packetHeader.packetFullSize);
	RxServerPacketHandler::I()->HandlePacket(buffer, numOfBytes);
}

void RxGameSession::ProcessSendImpl(uint32 numOfBytes)
{
	DEFAULT_TRACE_LOG("(%p 게임 세션)에게 (%d 바이트) 전송", this, numOfBytes);
}