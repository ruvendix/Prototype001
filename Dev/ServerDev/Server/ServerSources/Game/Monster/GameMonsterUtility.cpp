// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonsterUtility.h"

namespace
{
	struct AStarNode
	{
		int32 cost = 0; // 목적지까지의 비용
		Position2d cellPos; // 측정을 시작한 위치 (해당 위치부터 목적지까지가 cost)

		bool operator > (const AStarNode& other) const
		{
			return (cost > other.cost);
		}
	};

	uint32 CalculateHeuristic(const Position2d& srcCellPos, const Position2d& destCellPos)
	{
		return static_cast<uint32>(std::abs(destCellPos.x - srcCellPos.x) + std::abs(destCellPos.y - srcCellPos.y));
	}
}

namespace GameMonsterUtility
{
	bool CheckValidDistance(const Position2d& srcCellPos, const Position2d& destCellPos, uint32 dist)
	{
		uint32 srcToDestCost = CalculateHeuristic(srcCellPos, destCellPos);
		if (srcToDestCost > dist)
		{
			return false;
		}

		return true;
	}

	GamePlayerPtr FindNearestPlayer(const GameMonsterPtr& spMonster, uint32 dist)
	{
		ASSERT_LOG_RETURN_VALUE(spMonster != nullptr, nullptr);
		const Position2d& monsterCurrentCellPos = spMonster->MakeCurrentCellPosition();

		// 모든 플레이어로 전수 조사
		const std::unordered_map<uint64, GamePlayerPtr>& mapGamePlayer = GameRoom::I()->GetGamePlayers();
		for (const auto& iter : mapGamePlayer)
		{
			const GamePlayerPtr& spPlayer = iter.second;
			if ((spPlayer == nullptr) ||
				(spPlayer->CheckGameEntityState(Protocol::EEntityState::Death)))
			{
				continue;
			}

			const Position2d& playerCurrentCellPos = spPlayer->MakeCurrentCellPosition();

			// 유효한 거리 안에 있는지 확인
			if (CheckValidDistance(monsterCurrentCellPos, playerCurrentCellPos, dist) == true)
			{
				return spPlayer;
			}
		}

		return nullptr;
	}

	bool CalculateNavigationPath(const Position2d& srcCellPos, const Position2d& destCellPos, int32 chaseRange, std::vector<Position2d>& outVecNavigationPath)
	{
		// 추격 거리 안에 있는지 확인
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

				// 추격 거리 안에 있는지 확인
				if (chaseRange < nextAStarNode.cost)
				{
					continue;
				}

				// 갈 수 있는 곳인지 확인
				if (GameRoom::I()->CheckCanMoveToCellPosition(nextAStarNode.cellPos, nullptr) == false)
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
}