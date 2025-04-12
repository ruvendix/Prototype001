// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientPacketHandler.h"

#include "Scene/GameScene.h"

DEFINE_SINGLETON(ClientPacketHandler);

void ClientPacketHandler::Startup()
{
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::EnterGame, &ClientPacketHandler::HandlePacket_EnterGamePacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::LeaveGame, &ClientPacketHandler::HandlePacket_LeaveGamePacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::CreateMainPlayer, &ClientPacketHandler::HandlePacket_CreateMainPlayerPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::SyncEntities, &ClientPacketHandler::HandlePacket_SyncEntitiesPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::ModifyPlayerInformation, &ClientPacketHandler::HandlePacket_ModifyPlayerInformationPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::ModifyEntityLookAtDir, &ClientPacketHandler::HandlePacket_ModifyEntityLookAtDirectionPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::MoveEntity, &ClientPacketHandler::HandlePacket_MoveEntityPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::ModifyEntityState, &ClientPacketHandler::HandlePacket_ModifyEntityStatePacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::HitDamageToEntity, &ClientPacketHandler::HandlePacket_HitDamageToEntityPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::DieEntity, &ClientPacketHandler::HandlePacket_DieEntityPacket);
	REGISTER_PACKET_HANDLER(Protocol::EProtocolId::CreateProjectile, &ClientPacketHandler::HandlePacket_CreateProjectilePacket);
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

RxSendBufferPtr ClientPacketHandler::MakeModifyEntityLookAtDirectionPacket(const ProtocolEntityInfoPtr& spEntityInfo)
{
	Protocol::C_ModifyEntityLookAtDirectionPacket modifyEntityLookAtDirectionPacket;
	Protocol::EntityInfo* pEntityInfo = modifyEntityLookAtDirectionPacket.mutable_entity_info();
	pEntityInfo->set_entity_id(spEntityInfo->entity_id());
	pEntityInfo->set_entity_look_at_dir(spEntityInfo->entity_look_at_dir());

	return MakeSendBuffer(modifyEntityLookAtDirectionPacket, Protocol::EProtocolId::ModifyEntityLookAtDir);
}

RxSendBufferPtr ClientPacketHandler::MakeMoveEntityPacket(const ProtocolEntityInfoPtr& spEntityInfo)
{
	Protocol::C_MoveEntityPacket moveEntityPacket;
	Protocol::EntityInfo* pEntityInfo = moveEntityPacket.mutable_entity_info();
	pEntityInfo->set_entity_id(spEntityInfo->entity_id());
	pEntityInfo->set_entity_state(spEntityInfo->entity_state());
	pEntityInfo->set_cell_pos_x(spEntityInfo->cell_pos_x());
	pEntityInfo->set_cell_pos_y(spEntityInfo->cell_pos_y());

	return MakeSendBuffer(moveEntityPacket, Protocol::EProtocolId::MoveEntity);
}

RxSendBufferPtr ClientPacketHandler::MakeModifyEntityStatePacket(const ProtocolEntityInfoPtr& spEntityInfo)
{
	Protocol::C_ModifyEntityStatePacket modifyEntityStatePacket;
	Protocol::EntityInfo* pEntityInfo = modifyEntityStatePacket.mutable_entity_info();
	pEntityInfo->set_entity_id(spEntityInfo->entity_id());
	pEntityInfo->set_entity_state(spEntityInfo->entity_state());

	return MakeSendBuffer(modifyEntityStatePacket, Protocol::EProtocolId::ModifyEntityState);
}

RxSendBufferPtr ClientPacketHandler::MakeAttckToEntityPacket(const ProtocolEntityInfoPtr& spAttackerInfo, const ProtocolEntityInfoPtr& spVictimInfo)
{
	Protocol::C_AttckToEntityPacket attckToEntityPacket;
	Protocol::EntityInfo* pAttackerInfo = attckToEntityPacket.mutable_attacker_info();
	pAttackerInfo->set_entity_id(spAttackerInfo->entity_id());
	pAttackerInfo->set_entity_type(spAttackerInfo->entity_type());

	Protocol::EntityInfo* pVictimInfo = attckToEntityPacket.mutable_victim_info();
	pVictimInfo->set_entity_id(spVictimInfo->entity_id());
	pVictimInfo->set_entity_type(spVictimInfo->entity_type());

	return MakeSendBuffer(attckToEntityPacket, Protocol::EProtocolId::AttackToEntity);
}

RxSendBufferPtr ClientPacketHandler::MakeCreateProjectilePacket(const Position2d& spawnCellPos, const Vector2d& vMoveDir, int32 projectileId)
{
	Protocol::C_CreateProjectilePacket createProjectilePacket;
	Protocol::ProjectileInfo* pProjectileInfo = createProjectilePacket.mutable_projectile_info();
	pProjectileInfo->set_projectile_id(projectileId);

	Protocol::EntityInfo* pEntityInfo = pProjectileInfo->mutable_entity_info();
	pEntityInfo->set_cell_pos_x(spawnCellPos.x);
	pEntityInfo->set_cell_pos_y(spawnCellPos.y);

	Protocol::EEntityLookAtDirection entityLookAtDir = CommunicationActor::JudgeEntityLookAtDirection(vMoveDir);
	pEntityInfo->set_entity_look_at_dir(entityLookAtDir);

	return MakeSendBuffer(createProjectilePacket, Protocol::EProtocolId::CreateProjectile);
}

RxSendBufferPtr ClientPacketHandler::MakeDieEntityPacket(const ProtocolEntityInfoPtr& spEntityInfo)
{
	Protocol::C_DieEntityPacket dieEntityPacket;
	Protocol::EntityInfo* pVictimInfo = dieEntityPacket.mutable_victim_info();
	*pVictimInfo = *spEntityInfo;

	return MakeSendBuffer(dieEntityPacket, Protocol::EProtocolId::DieEntity);
}

void ClientPacketHandler::HandlePacket_EnterGamePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_EnterGamePacket, packet);
}

void ClientPacketHandler::HandlePacket_LeaveGamePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_LeaveGamePacket, packet);
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_LeaveGamePacket(packet);
}

void ClientPacketHandler::HandlePacket_CreateMainPlayerPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_CreateMainPlayerPacket, packet);

	// 받은 정보를 이용해서 플레이어를 생성!
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_CreateMainPlayerPacket(packet);
}

void ClientPacketHandler::HandlePacket_SyncEntitiesPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_SyncEntitiesPacket, packet);

	// 받은 정보를 이용해서 엔티티들 생성!
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_SyncEntitiesPacket(packet);
}

void ClientPacketHandler::HandlePacket_ModifyPlayerInformationPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_ModifyPlayerInformationPacket, packet);

	// 받은 정보를 이용해서 유저 정보 동기화
	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_ModifyPlayerInformationPacket(packet);
}

void ClientPacketHandler::HandlePacket_ModifyEntityLookAtDirectionPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_ModifyEntityLookAtDirectionPacket, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_ModifyEntityLookAtDirectionPacket(packet);
}

void ClientPacketHandler::HandlePacket_MoveEntityPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_MoveEntityPacket, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_MoveEntityPacket(packet);
}

void ClientPacketHandler::HandlePacket_ModifyEntityStatePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_ModifyEntityStatePacket, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_ModifyEntityStatePacket(packet);
}

void ClientPacketHandler::HandlePacket_HitDamageToEntityPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_HitDamageToEntityPacket, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_HitDamageToEntityPacket(packet);
}

void ClientPacketHandler::HandlePacket_DieEntityPacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_DieEntityPacket, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_DieEntityPacket(packet);
}

void ClientPacketHandler::HandlePacket_CreateProjectilePacket(BYTE* buffer, int32 numOfBytes)
{
	START_PACKET_CONTENTS(buffer, Protocol::S_CreateProjectilePacket, packet);

	GameScene* pGameScene = dynamic_cast<GameScene*>(SceneManager::I()->GetCurrentScene());
	ASSERT_LOG(pGameScene != nullptr);
	pGameScene->ParsingPacket_CreateProjectilePacket(packet);
}