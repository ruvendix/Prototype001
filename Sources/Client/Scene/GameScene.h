// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PlayerActor;

class GameScene : public Scene
{
public:
	using Super = Scene;

public:
	GameScene();
	virtual ~GameScene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

private:
	PlayerActor* m_pPlayerActor = nullptr;
};