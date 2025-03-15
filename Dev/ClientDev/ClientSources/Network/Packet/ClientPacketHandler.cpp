// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientPacketHandler.h"

#include "Scene/GameScene.h"

DEFINE_SINGLETON(ClientPacketHandler);

void ClientPacketHandler::Startup()
{
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::EnterGame, &ClientPacketHandler::HandlePacket_EnterGame);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::LeaveGame, &ClientPacketHandler::HandlePacket_LeaveGame);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::CreateLocalGamePlayer, &ClientPacketHandler::HandlePacket_CreateLocalGamePlayer);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntities, &ClientPacketHandler::HandlePacket_SyncGameEntities);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGamePlayer, &ClientPacketHandler::HandlePacket_SyncGamePlayer);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityLookAtDir, &ClientPacketHandler::HandlePacket_SyncGameEntityLookAtDirection);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGamePlayerMove, &ClientPacketHandler::HandlePacket_SyncGamePlayerMove);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityState, &ClientPacketHandler::HandlePacket_SyncGameEntityState);
}

void ClientPacketHandler::Cleanup()
{

}

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 numOfBytes)
{
	RxBufferReader bufferReader(buffer, numOfBytes, 0);

	RxPacketHeader packetHeader;
	bufferReader.Read(&packetHeader);

	// 구조상 쉽게
	auto foundIter = m_mapPacketHandler.find(packetHeader.protocolId);
	if (foundIter == m_mapPacketHandler.cend())
	{
		return;
	}

	foundIter->second.InvokeFunctions(buffer, numOfBytes);
}

RxSendBufferPtr ClientPacketHandler::MakeSyncGameEntityLookAtDirectionPacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGameEntityInfo)
{
	Protocol::C_SyncGameEntityLookAtDir syncGameEntityLookAtDir;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityLookAtDir.mutable_game_player_info();
	pGameEntityInfo->set_entity_id(spGameEntityInfo->entity_id());
	pGameEntityInfo->set_entitye_look_at_dir(spGameEntityInfo->entitye_look_at_dir());

	return MakeSendBuffer(syncGameEntityLookAtDir, Protocol::EProtocolId::SyncGameEntityLookAtDir);
}

RxSendBufferPtr ClientPacketHandler::MakeSyncGamePlayerMovePacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGamePlayerInfo)
{
	Protocol::C_SyncGamePlayerMove syncGamePlayerMovePacket;
	Protocol::GameEntityInfo* pGamePlayerInfo = syncGamePlayerMovePacket.mutable_game_player_info();
	pGamePlayerInfo->set_entity_id(spGamePlayerInfo->entity_id());
	pGamePlayerInfo->set_entity_state(spGamePlayerInfo->entity_state());
	pGamePlayerInfo->set_cell_pos_x(spGamePlayerInfo->cell_pos_x());
	pGamePlayerInfo->set_cell_pos_y(spGamePlayerInfo->cell_pos_y());

	return MakeSendBuffer(syncGamePlayerMovePacket, Protocol::EProtocolId::SyncGamePlayerMove);
}

RxSendBufferPtr ClientPacketHandler::MakeSyncGameEntityStatePacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGameEntityInfo)
{
	Protocol::C_SyncGamePlayerMove syncGameEntityStatePacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityStatePacket.mutable_game_player_info();
	pGameEntityInfo->set_entity_id(spGameEntityInfo->entity_id());
	pGameEntityInfo->set_entity_state(spGameEntityInfo->entity_state());

	return MakeSendBuffer(syncGameEntityStatePacket, Protocol::EProtocolId::SyncGameEntityState);
}

void ClientPacketHandler::HandlePacket_EnterGame(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_EnterGame, packet);
}

void ClientPacketHandler::HandlePacket_LeaveGame(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_LeaveGame, packet);
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_LeaveGamePlayer(packet);
}

void ClientPacketHandler::HandlePacket_CreateLocalGamePlayer(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_CreateLocalGamePlayer, packet);

	// 받은 정보를 이용해서 플레이어를 생성!
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_CreateLocalGamePlayer(packet);
}

void ClientPacketHandler::HandlePacket_SyncGameEntities(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGameEntities, packet);

	// 받은 정보를 이용해서 엔티티들 생성!
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_CreateGameEntities(packet);
}

void ClientPacketHandler::HandlePacket_SyncGamePlayer(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGamePlayer, packet);

	// 받은 정보를 이용해서 유저 정보 동기화
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGamePlayer(packet);
}

void ClientPacketHandler::HandlePacket_SyncGameEntityLookAtDirection(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGameEntityLookAtDir, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGameEntityLookAtDirection(packet);
}

void ClientPacketHandler::HandlePacket_SyncGamePlayerMove(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGamePlayerMove, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGamePlayerMove(packet);
}

void ClientPacketHandler::HandlePacket_SyncGameEntityState(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGameEntityState, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGameEntityState(packet);
}