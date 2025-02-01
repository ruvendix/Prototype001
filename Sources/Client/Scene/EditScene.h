// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class WorldBackgroundActor;
class WorldTileMapActor;
class EditorActor;

class EditScene : public Scene
{
public:
	using Super = Scene;

public:
	EditScene();
	virtual ~EditScene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

private:
	std::shared_ptr<WorldBackgroundActor> m_spWorldBackgroundActor = nullptr;
	std::shared_ptr<WorldTileMapActor> m_spWorldTileMapActor = nullptr;
	std::shared_ptr<EditorActor> m_spEditorActor = nullptr;
};