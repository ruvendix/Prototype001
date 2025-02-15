// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class EnemyRespawner;
class PlayerActor;

class EnemyMonsterActor : public AnimationActor
{
	using Super = AnimationActor;

public:
	using Super::Super;

	EnemyMonsterActor(const EnemyMonsterActor& srcActor);
	virtual ~EnemyMonsterActor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	
public:
	static bool CalculateNavigationPath(const Position2d& srcCellPos, const Position2d& destCellPos,
		int32 chaseRange, std::vector<Position2d>& outVecNavigationPath);

public:
	void DecreaseEnemyCountToEnemyRespawner() const;

	std::shared_ptr<PlayerActor> FindNearbyPlayerActor() const;
	std::shared_ptr<PlayerActor> FindChaseAvailablePlayerActor() const;

	void SetEnemyRespawner(const std::weak_ptr<EnemyRespawner>& spEnemyRespawner) { m_spEnemyRespawner = spEnemyRespawner; }
	const std::weak_ptr<EnemyRespawner>& GetEnemyRespawner() const { return m_spEnemyRespawner; }

	int32 GetChaseRange() const { return m_chaseRange; }

private:
	int32 m_chaseRange = 7;
	std::weak_ptr<EnemyRespawner> m_spEnemyRespawner;
};