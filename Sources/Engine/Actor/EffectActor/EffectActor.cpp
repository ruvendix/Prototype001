// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EffectActor.h"

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
}

bool EffectActor::Update(float deltaSeconds)
{
	if (Super::Update(deltaSeconds) == false)
	{
		return false;
	}

	// 각 기능은 컴포넌트로 분기
	DynamicSpriteComponent* pDynamicSpriteComponent = FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG(pDynamicSpriteComponent != nullptr);
	if (pDynamicSpriteComponent->IsAnimationEnd())
	{
		Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
		ASSERT_LOG_RETURN_VALUE(pCurrentScene != nullptr, false);
		pCurrentScene->ReserveEraseActor(shared_from_this());
		return true;
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

	DynamicSpriteComponent* pDynamicSpriteComponent = FindComponent<DynamicSpriteComponent>();
	ASSERT_LOG_RETURN(pDynamicSpriteComponent != nullptr);
	pDynamicSpriteComponent->ApplyDynamicSprite(spEffect->GetDynamicSprite());

	TransformComponent* pTransformComponent = BringTransformComponent();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetSize(effectSpawnInfo.effectSize);

	const Vec2d& vEffectSwpanWorldPos = CellActor::ConvertCellPositionToWorldPosition(effectSpawnInfo.spawnCellPos);
	pTransformComponent->SetPosition(vEffectSwpanWorldPos);
}