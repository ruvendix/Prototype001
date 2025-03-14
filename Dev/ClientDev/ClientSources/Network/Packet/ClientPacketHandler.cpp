// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientPacketHandler.h"

#include "Scene/GameScene.h"

DEFINE_SINGLETON(ClientPacketHandler);

void ClientPacketHandler::Startup()
{
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::EnterGame, &ClientPacketHandler::HandlePacket_EnterGame);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::CreateLocalGamePlayer, &ClientPacketHandler::HandlePacket_CreateLocalGamePlayer);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGameEntities, &ClientPacketHandler::HandlePacket_SyncGameEntities);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGamePlayer, &ClientPacketHandler::HandlePacket_SyncGamePlayer);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncGamePlayerMove, &ClientPacketHandler::HandlePacket_SyncGamePlayerMove);
}

void ClientPacketHandler::Cleanup()
{

}

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 numOfBytes)
{
	RxBufferReader bufferReader(buffer, numOfBytes, 0);

	RxPacketHeader packetHeader;
	bufferReader.Read(&packetHeader);

	// ������ ����
	auto foundIter = m_mapPacketHandler.find(packetHeader.protocolId);
	if (foundIter == m_mapPacketHandler.cend())
	{
		return;
	}

	foundIter->second.InvokeFunctions(buffer, numOfBytes);
}

RxSendBufferPtr ClientPacketHandler::MakeSyncGamePlayerMovePacket(const std::shared_ptr<Protocol::GameEntityInfo>& spGamePlayerInfo)
{
	Protocol::C_SyncGamePlayerMove syncGamePlayerMovePacket;
	Protocol::GameEntityInfo* pGamePlayerInfo = syncGamePlayerMovePacket.mutable_game_player_info();
	pGamePlayerInfo->set_entity_id(spGamePlayerInfo->entity_id());
	pGamePlayerInfo->set_cell_pos_x(spGamePlayerInfo->cell_pos_x());
	pGamePlayerInfo->set_cell_pos_y(spGamePlayerInfo->cell_pos_y());

	return MakeSendBuffer(syncGamePlayerMovePacket, Protocol::EProtocolId::SyncGamePlayerMove);
}

void ClientPacketHandler::HandlePacket_EnterGame(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_EnterGame, packet);
}

void ClientPacketHandler::HandlePacket_CreateLocalGamePlayer(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_CreateLocalGamePlayer, packet);

	// ���� ������ �̿��ؼ� �÷��̾ ����!
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_CreateLocalGamePlayer(packet);
}

void ClientPacketHandler::HandlePacket_SyncGameEntities(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGameEntities, packet);

	// ���� ������ �̿��ؼ� ��ƼƼ�� ����!
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_CreateGameEntities(packet);
}

void ClientPacketHandler::HandlePacket_SyncGamePlayer(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGamePlayer, packet);

	// ���� ������ �̿��ؼ� ���� ���� ����ȭ
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGamePlayer(packet);
}

void ClientPacketHandler::HandlePacket_SyncGamePlayerMove(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncGamePlayerMove, packet);

	// ���� ������ �̿��ؼ� ���� �̵� ���� ����ȭ
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncGamePlayerMove(packet);
}