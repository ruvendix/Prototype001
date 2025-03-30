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
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityMove, &ClientPacketHandler::HandlePacket_SyncGameEntityMove);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntityState, &ClientPacketHandler::HandlePacket_SyncGameEntityState);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::AttackToGameEntity, &ClientPacketHandler::HandlePacket_AttackToGameEntity);
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
	Protocol::C_SyncGameEntityLookAtDir syncGameEntityLookAtDirPacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityLookAtDirPacket.mutable_entity_info();
	pGameEntityInfo->set_entity_id(spGameEntityInfo->entity_id());
	pGameEntityInfo->set_entitye_look_at_dir(spGameEntityInfo->entitye_look_at_dir());

	return MakeSendBuffer(syncGameEntityLookAtDirPacket, Protocol::EProtocolId::SyncGameEntityLookAtDir);
}

RxSendBufferPtr ClientPacketHandler::MakeSyncGamePlayerMovePacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGamePlayerInfo)
{
	Protocol::C_SyncGameEntityMove syncGameEntityMovePacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityMovePacket.mutable_entity_info();
	pGameEntityInfo->set_entity_id(spGamePlayerInfo->entity_id());
	pGameEntityInfo->set_entity_state(spGamePlayerInfo->entity_state());
	pGameEntityInfo->set_cell_pos_x(spGamePlayerInfo->cell_pos_x());
	pGameEntityInfo->set_cell_pos_y(spGamePlayerInfo->cell_pos_y());

	return MakeSendBuffer(syncGameEntityMovePacket, Protocol::EProtocolId::SyncGameEntityMove);
}

RxSendBufferPtr ClientPacketHandler::MakeSyncGameEntityStatePacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGameEntityInfo)
{
	Protocol::C_SyncGameEntityState syncGameEntityStatePacket;
	Protocol::GameEntityInfo* pGameEntityInfo = syncGameEntityStatePacket.mutable_entity_info();
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

void ClientPacketHandler::HandlePacket_SyncGameEntityMove(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGameEntityMove, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGameEntityMove(packet);
}

void ClientPacketHandler::HandlePacket_SyncGameEntityState(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGameEntityState, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGameEntityState(packet);
}

void ClientPacketHandler::HandlePacket_AttackToGameEntity(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_AttckToGameEntity, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_AttackToGameEntity(packet);
}