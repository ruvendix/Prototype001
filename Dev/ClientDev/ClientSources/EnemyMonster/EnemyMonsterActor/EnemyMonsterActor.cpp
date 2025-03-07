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
	int32 cost = 0; // ������������ ���
	Position2d cellPos; // ������ ������ ��ġ (�ش� ��ġ���� ������������ cost)

	bool operator > (const AStarNode& other) const
	{
		return (cost > other.cost);
	}
};

bool EnemyMonsterActor::CalculateNavigationPath(const Position2d& srcCellPos, const Position2d& destCellPos,
	int32 chaseRange, std::vector<Position2d>& outVecNavigationPath)
{
	// ���� �Ÿ� �ȿ� �ִ��� Ȯ��
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

	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG(pCurrentScene != nullptr);

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

			// ���� �Ÿ� �ȿ� �ִ��� Ȯ��
			if (chaseRange < nextAStarNode.cost)
			{
				continue;
			}

			// �� �� �ִ� ������ Ȯ��
			if (pCurrentScene->CheckCanMoveToCellPosition(nextAStarNode.cellPos, nullptr) == false)
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

EnemyMonsterActor::EnemyMonsterActor(const EnemyMonsterActor& srcActor) : Super(srcActor)
{
	m_spEnemyRespawner = srcActor.m_spEnemyRespawner;
	ImmediatelyChangeState<EnmeyMonsterIdleState>(); // ����� �ƴ����� ����� ������ ��
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

	// ���� �Ÿ� �ȿ� �ִ��� Ȯ��
	for (const auto& spPlayerActor : vecPlayerActors)
	{
		// ���� �Ÿ� �ȿ� �ִ��� Ȯ��
		if (CheckInChaseRange(GetCellPosition(), spPlayerActor->GetCellPosition(), m_chaseRange) == true)
		{
			return spPlayerActor;
		}
	}

	return nullptr;
}