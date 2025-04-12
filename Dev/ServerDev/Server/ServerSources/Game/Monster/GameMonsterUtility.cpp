// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameMonsterUtility.h"

namespace
{
	struct AStarNode
	{
		int32 cost = 0; // ������������ ���
		Position2d cellPos; // ������ ������ ��ġ (�ش� ��ġ���� ������������ cost)

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

		// ��� �÷��̾�� ���� ����
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

			// ��ȿ�� �Ÿ� �ȿ� �ִ��� Ȯ��
			if (CheckValidDistance(monsterCurrentCellPos, playerCurrentCellPos, dist) == true)
			{
				return spPlayer;
			}
		}

		return nullptr;
	}

	bool CalculateNavigationPath(const Position2d& srcCellPos, const Position2d& destCellPos, int32 chaseRange, std::vector<Position2d>& outVecNavigationPath)
	{
		// �߰� �Ÿ� �ȿ� �ִ��� Ȯ��
		int32 srcToDestCost = CalculateHeuristic(srcCellPos, destCellPos);
		if (srcToDestCost > chaseRange)
		{
			return false;
		}

		std::map<Position2d, int32> mapBestPrimaryCost;
		std::map<Position2d, Position2d> mapParentCellPos; // �������� �ö󰡰� ��

#pragma region ���� ���
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

			// �������� ����
			if (astarNode.cellPos == destCellPos)
			{
				bFoundNavigationPath = true;
				break;
			}

			// ã�� ����� �� �۴ٸ� ����
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

				// �߰� �Ÿ� �ȿ� �ִ��� Ȯ��
				if (chaseRange < nextAStarNode.cost)
				{
					continue;
				}

				// �� �� �ִ� ������ Ȯ��
				if (GameRoom::I()->CheckCanMoveToCellPosition(nextAStarNode.cellPos, nullptr) == false)
				{
					continue;
				}

				// �̹� ��ϵ� ����� ����� �� ȿ�������� Ȯ��
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

		// ���ͳ��� �渷�ϸ� ��θ� �� ã�� �� ����
		if (bFoundNavigationPath == false)
		{
			int32 bestSecondaryCost = INT32_MAX;

			// mapBestPrimaryCost�� �ִ��� ã�� ��α����� ������ ����
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
			// ������
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