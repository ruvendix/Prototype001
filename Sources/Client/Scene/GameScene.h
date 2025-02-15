// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "WorldTileMapScene.h"

class PlayerActor;
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

private:
	std::shared_ptr<PlayerActor> m_spPlayerActor = nullptr;
	std::shared_ptr<SnakeActor> m_spSnakeActor = nullptr;

	std::shared_ptr<EnemyRespawner> m_spEnemyRespawner = nullptr;
};