// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "WorldTileMapScene.h"

class EditorActor;

class EditScene : public WorldTileMapScene
{
public:
	using Super = WorldTileMapScene;

public:
	EditScene();
	virtual ~EditScene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

private:
	std::shared_ptr<EditorActor> m_spEditorActor = nullptr;
};