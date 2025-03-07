// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameRoom.h"

DEFINE_SINGLETON(GameRoom);
DEFINE_COMPILETIMER_COUNTER(GameEntityIdCounter);

void GameRoom::Startup()
{

}

void GameRoom::Cleanup()
{

}

void GameRoom::EnterGameSession(const RxGameSessionPtr& spGameSession)
{
	uint32 newGameEntityId = GET_NEXT_COMPILEITME_ID(GameEntityIdCounter);
	ASSERT_LOG_RETURN(m_mapGamePlayer.find(newGameEntityId) == m_mapGamePlayer.cend());

	// Id ºÎ¿©
	GameEntityPtr spNewGameEntity = std::make_shared<GameEntity>();

	auto insertedIter = m_mapGamePlayer.insert(std::make_pair(newGameEntityId, spNewGameEntity));
	if (insertedIter.second == false)
	{
		DETAIL_ERROR_LOG(EErrorCode::ServerGameEntityInsertFailed);
		return;
	}

	const RxSendBufferPtr& spSendGameEntityInfo = RxServerPacketHandler::I()->MakeGameEntityInfoPacket(newGameEntityId);
	spGameSession->Send(spSendGameEntityInfo);
}