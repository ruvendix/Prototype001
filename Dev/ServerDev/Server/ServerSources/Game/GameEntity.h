// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameEntity : public EnableSharedClass
{
public:
	GameEntity();
	virtual ~GameEntity();

	virtual void Startup() override;
	virtual void Cleanup() override;

	Protocol::GameEntityInfo& GetGameEntityInfo() { return m_gameEntityInfo; }
	const Protocol::GameEntityInfo& GetGameEntityInfo() const { return m_gameEntityInfo; }

private:

	Protocol::GameEntityInfo m_gameEntityInfo;
};