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
	GameEntityActorPtr FindGameEntityActor(uint64 gameEntityId) const;

public:
	void ParsingPacket_LeaveGamePlayer(const Protocol::S_LeaveGame& leaveGamePacket);
	void ParsingPacket_CreateLocalGamePlayer(const Protocol::S_CreateLocalGamePlayer& createLocalGamePlayerPacket);
	void ParsingPacket_CreateGameEntities(const Protocol::S_SyncGameEntities& syncGameEntitiesPacket);
	void ParsingPacket_SyncGamePlayer(const Protocol::S_SyncGamePlayer& syncGamePlayerPacket);
	void ParsingPacket_SyncGameEntityLookAtDirection(const Protocol::S_SyncGameEntityLookAtDir& syncGameEntityLookAtDirPacket);
	void ParsingPacket_SyncGameEntityMove(const Protocol::S_SyncGameEntityMove& syncGamePlayerMovePacket);
	void ParsingPacket_SyncGameEntityState(const Protocol::S_SyncGameEntityState& syncGameEntityStatePacket);
	void ParsingPacket_AttackToGameEntity(const Protocol::S_AttckToGameEntity& attackToGameEntityPacket);
	
private:
	std::shared_ptr<LocalPlayerActor> m_spLocalPlayerActor = nullptr;

	std::shared_ptr<SnakeActor> m_spSnakeActor = nullptr;
	std::shared_ptr<EnemyRespawner> m_spEnemyRespawner = nullptr;
};