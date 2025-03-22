// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameMonsterActionState : public ICoreLoop
{
public:
	GameMonsterActionState(const std::shared_ptr<GameMonster>& spOwner);
	virtual ~GameMonsterActionState();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	virtual void OnActionTimer() = 0;

public:
	std::shared_ptr<GameMonster> BringOwner() const;

	void ModifyActionTriggerTime(float actionTriggerTime);
	void ResetActionTimer();

private:
	std::weak_ptr<GameMonster> m_spOwner;
	Timer m_actionTimer; // 몬스터는 일정 시간마다 행동
};

class GameMonsterIdleState : public GameMonsterActionState
{
	using Super = GameMonsterActionState;
	using Super::Super;

public:
	virtual ~GameMonsterIdleState() = default;

	virtual void Startup() override;
	virtual void OnActionTimer() override;

private:
	void ProcessRandomMove();
};

class GameMonsterWalkState : public GameMonsterActionState
{
	using Super = GameMonsterActionState;
	using Super::Super;

public:
	virtual ~GameMonsterWalkState() = default;

	virtual void Startup() override;
	virtual void OnActionTimer() override;
};

class GameMonsterChaseState : public GameMonsterActionState
{
	using Super = GameMonsterActionState;
	using Super::Super;

public:
	virtual ~GameMonsterChaseState() = default;

	virtual void Startup() override;
	virtual void OnActionTimer() override;

private:
	bool ProcessNavigationPath(std::vector<Position2d>& outVecNavigationPos) const;
	void ProcessNavigationMove(const Position2d& moveNavigationPos) const;
};

class GameMonsterAttackState : public GameMonsterActionState
{
	using Super = GameMonsterActionState;
	using Super::Super;

public:
	virtual ~GameMonsterAttackState() = default;

	virtual void Startup() override;
	virtual void OnActionTimer() override;
};