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

void ProjectileActor::ProcessDamaged(const std::shared_ptr<PawnActor>& spAttacker)
{
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);

	// 이펙트 생성
	EffectSpawnInfo effectSpawnInfo;
	effectSpawnInfo.strEffectName = "OneTime_HitEffect";
	effectSpawnInfo.spawnCellPos = GetCellPosition();
	effectSpawnInfo.effectSize = Size(80, 80);
	pCurrentScene->ReserveCreateEffectActor(effectSpawnInfo);

	pCurrentScene->ReserveEraseActor(spAttacker); // 투사체는 자기 자신한테만 피해를 받음
}

void ProjectileActor::SpawnProjectile(const ProjectileSpawnInfo& projectileSpawnInfo)
{
	TransformComponent* pTransformComponent = GetComponent<TransformComponent>();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetSize(projectileSpawnInfo.projectileSize);

	// 투사체는 이동하는 기능이 있음
	CellActorMoveComponent* pCellActorMoveComponent = GetComponent<CellActorMoveComponent>();
	ASSERT_LOG_RETURN(pCellActorMoveComponent != nullptr);
	pCellActorMoveComponent->SetMoveSpeed(projectileSpawnInfo.flyingSpeed);
	pCellActorMoveComponent->ProcessMoveDirection(projectileSpawnInfo.vMoveDir, true);
	
	ChangeActorStateDynamicSprite<PawnActorIdleState>();

	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);
	pCurrentScene->AddActor(SharedFromThisExactType<ProjectileActor>(), EActorLayerType::WorldForeground);
}