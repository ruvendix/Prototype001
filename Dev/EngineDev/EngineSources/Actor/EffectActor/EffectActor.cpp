// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EffectActor.h"

#include "EngineSources/Component/EffectComponent/FollowerEffectComponent.h"
#include "EngineSources/Component/EffectComponent/LifeTimeEffectComponent.h"

EffectActor::EffectActor()
{
	
}

EffectActor::~EffectActor()
{

}

void EffectActor::Startup()
{
	Super::Startup();

	AddComponent<DynamicSpriteComponent>();
	AddComponent<LifeTimeEffectComponent>();
}

bool EffectActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	return true;
}

void EffectActor::Cleanup()
{
	Super::Cleanup();
}

void EffectActor::SpawnEffect(const EffectSpawnInfo& effectSpawnInfo)
{
	const EffectPtr& spEffect = ResourceMananger::I()->FindEffect(effectSpawnInfo.strEffectName);
	ASSERT_LOG_RETURN(spEffect != nullptr);

	DynamicSpriteComponent* pDynamicSpriteComponent = GetComponent<DynamicSpriteComponent>();
	ASSERT_LOG_RETURN(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spEffect->GetDynamicSprite());

	TransformComponent* pTransformComponent = GetComponent<TransformComponent>();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetSize(effectSpawnInfo.effectSize);

	ApplyCellPosition(effectSpawnInfo.spawnCellPos);

	LifeTimeEffectComponent* pLifeTimeEffectComponent = GetComponent<LifeTimeEffectComponent>();
	ASSERT_LOG_RETURN(pLifeTimeEffectComponent != nullptr);

	// LifeTime을 따로 설정하지 않았다면 DynamicSprite의 재생 시간을 적용
	float dynamicSpriteDurationTime = pDynamicSpriteComponent->BringDynamicSpriteDurationTime();
	if (effectSpawnInfo.lifeTime > dynamicSpriteDurationTime)
	{
		dynamicSpriteDurationTime = effectSpawnInfo.lifeTime;
	}
	pLifeTimeEffectComponent->SetLifeTime(dynamicSpriteDurationTime);

	if (effectSpawnInfo.spFollowTargetActor != nullptr)
	{
		FollowerEffectComponent* pFollowerEffectComponent = AddComponent<FollowerEffectComponent>();
		ASSERT_LOG_RETURN(pFollowerEffectComponent != nullptr);
		pFollowerEffectComponent->SetFollowTargetActor(effectSpawnInfo.spFollowTargetActor);
	}
}