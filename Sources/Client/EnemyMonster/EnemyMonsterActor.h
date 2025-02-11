// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class EnemyRespawner;

class EnemyMonsterActor : public AnimationActor
{
	using Super = AnimationActor;

public:
	using Super::Super;

	EnemyMonsterActor(const EnemyMonsterActor& srcActor);
	virtual ~EnemyMonsterActor();

public:
	void DecreaseEnemyCountToEnemyRespawner();

	void SetEnemyRespawner(const std::weak_ptr<EnemyRespawner>& spEnemyRespawner) { m_spEnemyRespawner = spEnemyRespawner; }
	const std::weak_ptr<EnemyRespawner>& GetEnemyRespawner() const { return m_spEnemyRespawner; }

private:
	std::weak_ptr<EnemyRespawner> m_spEnemyRespawner;
};