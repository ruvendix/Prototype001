// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "WorldTileMapScene.h"

class LocalPlayerActor;
class SnakeActor;

class GameScene : public WorldTileMapScene
{
public:
	using Super = WorldTileMapScene;

public:
	GameScene();
	virtual ~GameScene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	NetworkEntityActorPtr FindNetworkEntityActor(uint64 networkEntityId) const;

public:
	void ParsingPacket_LeaveGamePacket(const Protocol::S_LeaveGamePacket& leaveGamePacket);
	void ParsingPacket_CreateMainPlayerPacket(const Protocol::S_CreateMainPlayerPacket& createMainPlayerPacket);
	void ParsingPacket_SyncEntitiesPacket(const Protocol::S_SyncEntitiesPacket& syncEntitiesPacket);
	void ParsingPacket_ModifyPlayerInformationPacket(const Protocol::S_ModifyPlayerInformationPacket& modifyPlayerInformationPacket) const;
	void ParsingPacket_ModifyEntityLookAtDirectionPacket(const Protocol::S_ModifyEntityLookAtDirectionPacket& modifyEntityLookAtDirectionPacket) const;
	void ParsingPacket_MoveEntityPacket(const Protocol::S_MoveEntityPacket& moveEntityPacket) const;
	void ParsingPacket_ModifyEntityStatePacket(const Protocol::S_ModifyEntityStatePacket& modifyEntityStatePacket) const;
	void ParsingPacket_HitDamageToEntityPacket(const Protocol::S_HitDamageToEntityPacket& hitDamageToEntityPacket) const;
	void ParsingPacket_DiePlayerPacket(const Protocol::S_DiePlayerPacket& diePlayerPacket);
	
private:
	std::shared_ptr<LocalPlayerActor> m_spLocalPlayerActor = nullptr;

	std::shared_ptr<SnakeActor> m_spSnakeActor = nullptr;
	std::shared_ptr<EnemyRespawner> m_spEnemyRespawner = nullptr;
};