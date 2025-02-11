// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class WorldBackgroundActor;
class WorldTileMapActor;
class PlayerActor;
class SnakeActor;

class GameScene : public Scene
{
public:
	using Super = Scene;

public:
	GameScene();
	virtual ~GameScene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	bool CheckCanMoveToCellPosition(const Position2d& destCellPos) const override;

private:
	std::shared_ptr<WorldBackgroundActor> m_spWorldBackgroundActor = nullptr;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
	std::shared_ptr<PlayerActor> m_spPlayerActor = nullptr;
	std::shared_ptr<SnakeActor> m_spSnakeActor = nullptr;

	std::shared_ptr<EnemyRespawner> m_spEnemyRespawner = nullptr;
};