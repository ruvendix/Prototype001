// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ProjectileActor.h"

#include "ProjectileAttackComponent.h"

ProjectileActor::ProjectileActor(const ProjectileActor& srcActor) :
	Super(srcActor)
{

}

ProjectileActor::~ProjectileActor()
{

}

void ProjectileActor::Startup()
{
	Super::Startup();
	SetActorLayer(EActorLayerType::WorldForeground);
	
	// 필요한 컴포넌트 추가
	AddComponent<DynamicSpriteComponent>();
	AddComponent<CellActorMoveComponent>();
	AddComponent<ProjectileAttackComponent>();
}

bool ProjectileActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void ProjectileActor::Cleanup()
{
	Super::Cleanup();
}

ActorPtr ProjectileActor::CreateClone()
{
	return (std::make_shared<ProjectileActor>(*this));
}

void ProjectileActor::SpawnProjectile(const ProjectileSpawnInfo& projectileSpawnInfo)
{
	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetSize(projectileSpawnInfo.projectileSize);

	ApplyCellPosition(projectileSpawnInfo.spawnCellPos);

	// 투사체는 이동하는 기능이 있음
	CellActorMoveComponent* pCellActorMoveComponent = FindComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pCellActorMoveComponent != nullptr);
	pCellActorMoveComponent->SetMoveSpeed(projectileSpawnInfo.flyingSpeed);
	pCellActorMoveComponent->SetDestinationCellPosition(projectileSpawnInfo.spawnCellPos);
	pCellActorMoveComponent->ProcessMoveDirection(projectileSpawnInfo.vMoveDir, true);
	
	ChangeActorStateDynamicSprite<PawnActorIdleState>();
}