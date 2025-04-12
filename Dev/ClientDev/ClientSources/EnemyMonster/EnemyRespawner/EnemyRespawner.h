// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

DECLARE_COMPILETIMER_COUNTER(EnemeyActorIdCounter);

class EnemyRespawner : public EnableSharedClass
{
public:
	EnemyRespawner();
	virtual ~EnemyRespawner();

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void RespawnEnemy(const Protocol::MonsterInfo& monsterInfo, Scene* pCurrentScene);

public:
	template <typename TEnemyActor>
	void AddPrototypeEnemyActor(const std::shared_ptr<TEnemyActor>& spPrototypeEnemyActor)
	{
		static_assert(std::is_base_of_v<EnemyMonsterActor, TEnemyActor> == true, "TEnemyActor isn't derived EnemyMonsterActor");

		auto insertedIter = m_mapPrototypeEnemyActors.insert(std::make_pair(TEnemyActor::s_id, spPrototypeEnemyActor));
		if (insertedIter.second == false)
		{
			DEFAULT_TRACE_LOG("Id가 %d인 적군의 프로토타입은 이미 존재!", TEnemyActor::s_id);
		}
	}

private:
	std::unordered_map<int32, std::shared_ptr<EnemyMonsterActor>> m_mapPrototypeEnemyActors; // 리스폰 전용 프로토타입 적군들 (씬에 안 넣음)
};