// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameEntity : public EnableSharedClass
{
public:
	GameEntity();
	~GameEntity();

	virtual void Startup() override;
	virtual void Cleanup() override;

private:
	//Protocol::S_GameEntityInfo m_gameEntityInfo;
};