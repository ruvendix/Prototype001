// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameMonster : public GameEntity
{
public:
	GameMonster();
	virtual ~GameMonster();

	virtual void Startup() override;
	virtual void Cleanup() override;

public:
	void ApplyGameMonsterInfo(const Protocol::GameMonsterInfo& srcGameMonsterInfo);
	void CopyGameMonsterInfo(Protocol::GameMonsterInfo* pDestGameMonsterInfo);

private:
	uint32 m_monsterId = 0;
};