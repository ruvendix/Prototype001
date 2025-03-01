// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterActor.h"

#include "EngineSources/Actor/PawnActor/PawnActorState.h"
#include "ClientSources/Player/PlayerActor.h"
#include "ClientSources/EnemyMonster/EnemyMonsterState/EnemyMonsterState.h"
#include "ClientSources/EnemyMonster/EnemyMonsterComponent/EnemyMonsterAttackComponent.h"

namespace
{
	int32 CalculateHeuristic(const Position2d& srcCellPos, const Position2d& destCellPos)
	{
		return (std::abs(destCellPos.x - srcCellPos.x) + std::abs(destCellPos.y - srcCellPos.y));
	}

	bool CheckInChaseRange(const Position2d& srcCellPos, const Position2d& destCellPos, int32 chaseRange)
	{
		int32 srcToDestCost = CalculateHeuristic(srcCellPos, destCellPos);
		if (srcToDestCost > chaseRange)
		{
			return false;
		}

		return true;
	}
}

struct AStarNode
{
	int32 cost = 0; // 목적지까지의 비용
	Position2d cellPos; // 측정을 시작한 위치 (해당 위치부터 목적지까지가 cost)

	bool operator > (const AStarNode& other) const
	{
		return (cost > other.cost);
	}
};

bool EnemyMonsterActor::CalculateNavigationPath(const Position2d& srcCellPos, const Position2d& destCellPos,
	int32 chaseRange, std::vector<Position2d>& outVecNavigationPath)
{
	// 추적 거리 안에 있는지 확인
	int32 srcToDestCost = CalculateHeuristic(srcCellPos, destCellPos);
	if (srcToDestCost > chaseRange)
	{
		return false;
	}

	std::map<Position2d, int32> mapBestPrimaryCost;
	std::map<Position2d, Position2d> mapParentCellPos; // 역순으로 올라가게 됨

#pragma region 시작 노드
	AStarNode srcAStarNode;
	srcAStarNode.cost = srcToDestCost;
	srcAStarNode.cellPos = srcCellPos;

	auto insertedBestPrimaryCostIter = mapBestPrimaryCost.insert(std::make_pair(srcAStarNode.cellPos, srcAStarNode.cost));
	ASSERT_LOG(insertedBestPrimaryCostIter.second == true);

	auto insertedParentCellPosIter = mapParentCellPos.insert(std::make_pair(srcAStarNode.cellPos, srcAStarNode.cellPos));
	ASSERT_LOG(insertedParentCellPosIter.second == true);
#pragma endregion

	std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> priorityQueueAStar;
	priorityQueueAStar.push(srcAStarNode);

	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG(pCurrentScene != nullptr);

	bool bFoundNavigationPath = false;
	while (priorityQueueAStar.empty() == false)
	{
		AStarNode astarNode = priorityQueueAStar.top();
		priorityQueueAStar.pop();

		// 목적지면 종료
		if (astarNode.cellPos == destCellPos)
		{
			bFoundNavigationPath = true;
			break;
		}

		// 찾은 비용이 더 작다면 생략
		auto foundIter = mapBestPrimaryCost.find(astarNode.cellPos);
		ASSERT_LOG(foundIter != mapBestPrimaryCost.cend());
		if (foundIter->second < astarNode.cost)
		{
			continue;
		}

		for (const Position2d& dirCellPos : PawnActor::g_lookAtForwardCellPosTable)
		{
			AStarNode nextAStarNode;
			nextAStarNode.cellPos = (astarNode.cellPos + dirCellPos);
			nextAStarNode.cost = CalculateHeuristic(nextAStarNode.cellPos, destCellPos);

			// 추적 거리 안에 있는지 확인
			if (chaseRange < nextAStarNode.cost)
			{
				continue;
			}

			// 갈 수 있는 곳인지 확인
			if (pCurrentScene->CheckCanMoveToCellPosition(nextAStarNode.cellPos, nullptr) == false)
			{
				continue;
			}

			// 이미 등록된 노드라면 비용이 더 효율적인지 확인
			auto foundBestCostIter = mapBestPrimaryCost.find(nextAStarNode.cellPos);
			if (foundBestCostIter != mapBestPrimaryCost.cend())
			{
				if (foundBestCostIter->second <= nextAStarNode.cost)
				{
					continue;
				}
			}

			insertedBestPrimaryCostIter = mapBestPrimaryCost.insert(std::make_pair(nextAStarNode.cellPos, nextAStarNode.cost));
			ASSERT_LOG(insertedBestPrimaryCostIter.second == true);

			insertedParentCellPosIter = mapParentCellPos.insert(std::make_pair(nextAStarNode.cellPos, astarNode.cellPos));
			ASSERT_LOG(insertedParentCellPosIter.second == true);

			priorityQueueAStar.push(nextAStarNode);
		}
	}

	Position2d resultDestCellPos = destCellPos;

	// 액터끼리 길막하면 경로를 못 찾을 수 있음
	if (bFoundNavigationPath == false)
	{
		int32 bestSecondaryCost = INT32_MAX;

		// mapBestPrimaryCost는 최대한 찾은 경로까지의 정보가 있음
		for (const auto& bestPrimaryCostIter : mapBestPrimaryCost)
		{
			if (bestPrimaryCostIter.second == bestSecondaryCost)
			{
				int32 dist1 = CalculateHeuristic(srcCellPos, resultDestCellPos);
				int32 dist2 = CalculateHeuristic(srcCellPos, bestPrimaryCostIter.first);

				if (dist1 > dist2)
				{
					resultDestCellPos = bestPrimaryCostIter.first;
				}
			}
			else if (bestPrimaryCostIter.second < bestSecondaryCost)
			{
				resultDestCellPos = bestPrimaryCostIter.first;
				bestSecondaryCost = bestPrimaryCostIter.second;
			}
		}
	}

	Position2d foundPathCellPos = resultDestCellPos;

	outVecNavigationPath.clear();
	while (true)
	{
		// 시작점
		auto foundParentCellPosIter = mapParentCellPos.find(foundPathCellPos);
		ASSERT_LOG(foundParentCellPosIter != mapParentCellPos.cend());
		if (foundPathCellPos == foundParentCellPosIter->second)
		{
			break;
		}

		outVecNavigationPath.push_back(foundPathCellPos);
		foundPathCellPos = foundParentCellPosIter->second;
	}

	std::reverse(outVecNavigationPath.begin(), outVecNavigationPath.end());
	return true;
}

EnemyMonsterActor::EnemyMonsterActor(const EnemyMonsterActor& srcActor) : Super(srcActor)
{
	m_spEnemyRespawner = srcActor.m_spEnemyRespawner;
	ImmediatelyChangeState<EnmeyMonsterIdleState>(); // 복사는 아니지만 비슷한 역할을 함
}

EnemyMonsterActor::~EnemyMonsterActor()
{
	
}

void EnemyMonsterActor::Startup()
{
	Super::Startup();
	AddComponent<CellActorMoveComponent>();
	AddComponent<EnemyMonsterAttackComponent>();
	ImmediatelyChangeState<EnmeyMonsterIdleState>();
}

bool EnemyMonsterActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

bool EnemyMonsterActor::CheckMovingState() const
{
	return (IsSamePawnActorState<EnmeyMonsterChaseState>());
}

void EnemyMonsterActor::DecreaseEnemyCountToEnemyRespawner() const
{
	if (m_spEnemyRespawner.expired() == false)
	{
		m_spEnemyRespawner.lock()->DecreaseEnemyCount();
	}
}

std::shared_ptr<PlayerActor> EnemyMonsterActor::FindNearbyPlayerActor() const
{
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG(pCurrentScene != nullptr);

	std::vector<std::shared_ptr<PlayerActor>> vecFoundPlayerActors;
	pCurrentScene->FindExactTypeActors(EActorLayerType::Creature, vecFoundPlayerActors);
	if (vecFoundPlayerActors.empty() == true)
	{
		return nullptr;
	}

	for (const std::shared_ptr<PlayerActor>& spPlayerActor : vecFoundPlayerActors)
	{
		ASSERT_LOG(spPlayerActor != nullptr);

		for (const Position2d& dirCellPos : PawnActor::g_lookAtForwardCellPosTable)
		{
			Position2d nearbyCellPos = (GetCellPosition() + dirCellPos);
			if (spPlayerActor->CheckEqaulCellPosition(nearbyCellPos) == true)
			{
				return spPlayerActor;
			}
		}
	}

	return nullptr;
}

std::shared_ptr<PlayerActor> EnemyMonsterActor::FindChaseAvailablePlayerActor() const
{
	const Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG(pCurrentScene != nullptr);

	std::vector<std::shared_ptr<PlayerActor>> vecPlayerActors;
	pCurrentScene->FindExactTypeActors<PlayerActor>(EActorLayerType::Creature, vecPlayerActors);
	if (vecPlayerActors.empty() == true)
	{
		return nullptr;
	}

	// 추적 거리 안에 있는지 확인
	for (const auto& spPlayerActor : vecPlayerActors)
	{
		// 추적 거리 안에 있는지 확인
		if (CheckInChaseRange(GetCellPosition(), spPlayerActor->GetCellPosition(), m_chaseRange) == true)
		{
			return spPlayerActor;
		}
	}

	return nullptr;
}