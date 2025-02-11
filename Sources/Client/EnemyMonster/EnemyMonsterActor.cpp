// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EnemyMonsterActor.h"

EnemyMonsterActor::EnemyMonsterActor(const EnemyMonsterActor& srcActor) : Super(srcActor)
{
	m_spEnemyRespawner = srcActor.m_spEnemyRespawner;
}

EnemyMonsterActor::~EnemyMonsterActor()
{
	
}

void EnemyMonsterActor::DecreaseEnemyCountToEnemyRespawner()
{
	if (m_spEnemyRespawner.expired() == false)
	{
		m_spEnemyRespawner.lock()->DecreaseEnemyCount();
	}
}