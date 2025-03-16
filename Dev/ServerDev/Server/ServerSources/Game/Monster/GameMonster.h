// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameMonster : public GameEntity
{
public:
	GameMonster();
	virtual ~GameMonster();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void UpdateIdle(float deltaSeconds);
	virtual void UpdateWalk(float deltaSeconds);

public:
	void ApplyGameMonsterInfo(const Protocol::GameMonsterInfo& srcGameMonsterInfo);
	void CopyGameMonsterInfo(Protocol::GameMonsterInfo* pDestGameMonsterInfo);

private:
	void OnTimerIdle();
	void OnTimerWalk();

private:
	uint32 m_monsterId = 0;
	Timer m_timerOnIdle;
	Timer m_timerOnWalk;
};