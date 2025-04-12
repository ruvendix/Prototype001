// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class GameMonsterActionState;

class GameMonster : public GameEntity
{
public:
	GameMonster();
	virtual ~GameMonster();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void AddToSyncEntitiesPacket(Protocol::S_SyncEntitiesPacket& outSyncEntitiesPacket) override;

public:
	template <typename TState>
	std::shared_ptr<TState> ReserveNextState()
	{
		static_assert((std::is_base_of_v<GameMonsterActionState, TState> == true), "TState isn't base of GameMonsterActionState!");
		const std::shared_ptr<TState>& spNextActionState = std::make_shared<TState>(SharedFromThisExactType<GameMonster>());
		spNextActionState->Startup();
		m_reserveNextStateEvent.RegisterEventHandler([=](){ m_spActionState = spNextActionState; });
		return spNextActionState;
	}

public:
	void ApplyGameMonsterInfo(const Protocol::MonsterInfo& srcMonsterInfo);
	void CopyGameMonsterInfo(Protocol::MonsterInfo* pDestMonsterInfo);

	uint32 GetChaseRange() const { return m_chaseRange; }
	uint32 GetAttackableRange() const { return m_attackableRange; }

private:
	uint32 m_monsterId = 0;

	uint32 m_chaseRange = 7;
	uint32 m_attackableRange = 1;

	Event<void> m_reserveNextStateEvent;
	std::shared_ptr<GameMonsterActionState> m_spActionState = nullptr;
};