// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "LifeTimeEffectComponent.h"

DEFINE_COMPILETIME_ID(LifeTimeEffectComponent, ComponentIdCounter)

void LifeTimeEffectComponent::Startup()
{
	m_lifeTimer = TimerManager::I()->CreateTimer(0.0f, false, this, &LifeTimeEffectComponent::OnLifeTimeEnd);
}

bool LifeTimeEffectComponent::Update(float deltaSeconds)
{
	m_lifeTimer.Update(deltaSeconds);
	return true;
}

void LifeTimeEffectComponent::OnLifeTimeEnd()
{
	Scene* pCurrentScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pCurrentScene != nullptr);
	pCurrentScene->ReserveEraseActor(GetOwner()->SharedFromThisExactType<Actor>());
}