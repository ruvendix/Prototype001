// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WorldBackgroundActor;
class WorldTileMapActor;

class WorldTileMapScene : public Scene
{
public:
	using Super = Scene;

public:
	WorldTileMapScene();
	virtual ~WorldTileMapScene();

	virtual void Startup() override;

public:
	bool CheckCanMoveToCellPosition(const Position2d& destCellPos, const Actor* pExcludeActor) const override;

	const std::shared_ptr<WorldTileMapActor>& GetWorldTileMapActor() const { return m_spWorldTileMapActor; }

private:
	std::shared_ptr<WorldBackgroundActor> m_spWorldBackgroundActor = nullptr;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
};