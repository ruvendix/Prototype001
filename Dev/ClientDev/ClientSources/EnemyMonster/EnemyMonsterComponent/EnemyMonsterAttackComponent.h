// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class EnemyMonsterAttackComponent : public Component
{
	DECLARE_COMPILETIME_ID;

public:
	EnemyMonsterAttackComponent();
	virtual ~EnemyMonsterAttackComponent();

	virtual bool Update(float deltaSeconds) override;
	virtual ComponentPtr CreateClone() override;

public:
	void ActivateAttackIfAttackable(const std::shared_ptr<PlayerActor>& spVictimPlayerActor);
	void SetAttackDelayTime(float attackDelayTime) { m_attackTimer.SetTriggerTime(attackDelayTime); }

	void OffAttackTimer();

private:
	void OnAttackToVictimPlayerActor(const std::shared_ptr<PlayerActor>& spVictimPlayerActor);

private:
	Timer m_attackTimer;
};