// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameSession.h"

#include "GameSessionManager.h"
#include "Packet/ServerPacketHandler.h"

DEFINE_COMPILETIMER_COUNTER(UserIdCounter);

RxGameSession::RxGameSession()
{
	DEFAULT_TRACE_LOG("(%p ���� ����) ����!\n", this);
}

RxGameSession::~RxGameSession()
{
	DEFAULT_TRACE_LOG("(%p ���� ����) �Ҹ�!", this);
}

void RxGameSession::ProcessConnectImpl()
{
	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->AddGameSession(spGameSession);

	// Ŭ���̾�Ʈ ����!
	m_userId = GET_NEXT_COMPILEITME_ID(UserIdCounter);
	Send(RxServerPacketHandler::I()->MakeEnterGamePacket(m_userId));

	// Ŭ���̾�Ʈ�� �濡 ����!
	GameRoom::I()->EnterGameRoom(spGameSession);
	DEFAULT_TRACE_LOG("(%p ���� ����) ����", spGameSession);
}

void RxGameSession::ProcessDisconnectImpl()
{
	// �������Ƿ� ��� ���� ���ǿ��� �˸�
	RxGameSessionManager::I()->Broadcast(RxServerPacketHandler::I()->MakeLeaveGamePacket(m_userId, m_spGamePlayer.lock()));

	const RxGameSessionPtr& spGameSession = SharedFromThisExactType<RxGameSession>();
	RxGameSessionManager::I()->RemoveGameSession(spGameSession);

	// Ŭ���̾�Ʈ�� �濡�� ������!
	GameRoom::I()->LeaveGameRoom(spGameSession);
	DEFAULT_TRACE_LOG("(%p ���� ����) ����", spGameSession);
}

void RxGameSession::ProcessReceivePacket(BYTE* buffer, uint32 numOfBytes)
{
	RxPacketHeader packetHeader = *(reinterpret_cast<RxPacketHeader*>(buffer));
	DEFAULT_TRACE_LOG("���� ��Ŷ ���(Id: %d, Size: %d)", packetHeader.protocolId, packetHeader.packetFullSize);
	RxServerPacketHandler::I()->HandlePacket(buffer, numOfBytes);
}

void RxGameSession::ProcessSendImpl(uint32 numOfBytes)
{
	DEFAULT_TRACE_LOG("(%p ���� ����)���� (%d ����Ʈ) ����", this, numOfBytes);
}