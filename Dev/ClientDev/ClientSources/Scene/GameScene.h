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
	void ParsingPacket_CreateLocalGamePlayer(const Protocol::S_CreateLocalGamePlayer& createLocalGamePlayerPacket);
	void ParsingPacket_CreateGameEntities(const Protocol::S_SyncGameEntities& syncGameEntities);
	void ParsingPacket_SyncGamePlayer(const Protocol::S_SyncGamePlayer& syncGamePlayer);
	void ParsingPacket_SyncGamePlayerMove(const Protocol::S_SyncGamePlayerMove& syncGamePlayerMove);

private:
	std::shared_ptr<LocalPlayerActor> m_spLocalPlayerActor = nullptr;

	std::shared_ptr<SnakeActor> m_spSnakeActor = nullptr;
	std::shared_ptr<EnemyRespawner> m_spEnemyRespawner = nullptr;
};