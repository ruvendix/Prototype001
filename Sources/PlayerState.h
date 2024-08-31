#pragma once

#include "IGameDefault.h"
#include "CompilerCounter.hpp"
#include "PlayerActor.h"

class PlayerStateBase : public IGameDefault
{
public:
	using PlayerDirectionStrings = std::array<std::string, TO_NUM(EPlayerDirection::Count)>;

public:
	PlayerStateBase(PlayerActor* pOwner);
	virtual ~PlayerStateBase();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void ChangePlayerFlipbookOnDirection(const PlayerDirectionStrings& playerDirStrs);

	PlayerActor* GetOwner() const { return m_pOwner; }

private:
	PlayerActor* m_pOwner = nullptr;
};

class PlayerIdleState : public PlayerStateBase
{
public:
	using PlayerStateBase::PlayerStateBase;
	virtual ~PlayerIdleState();

	virtual bool Update() override;
	bool ProcessIdleInput();
};

class PlayerWalkState : public PlayerStateBase
{
public:
	using PlayerStateBase::PlayerStateBase;
	virtual ~PlayerWalkState();

	virtual bool Update() override;
	bool ProcessWalkState();
};

class PlayerStateEnd : public PlayerStateBase
{
public:
	using PlayerStateBase::PlayerStateBase;
	virtual ~PlayerStateEnd();
};

DEFINE_COMPILER_COUNTER(PlayerStateIdCounter);

DEFINE_ID(PlayerStateIdCounter, PlayerIdleState);
DEFINE_ID(PlayerStateIdCounter, PlayerWalkState);
DEFINE_ID(PlayerStateIdCounter, PlayerStateEnd);