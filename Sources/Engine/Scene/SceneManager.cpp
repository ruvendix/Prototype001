// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneManager.h"

DEFINE_SINGLETON(SceneManager);

void SceneManager::Startup()
{

}

bool SceneManager::Update(float deltaSeconds)
{
	m_sceneChangeEvent.ExcuteIfBound();
	// m_sceneChangeEvent.RegisterEventHandler(this, &SceneManager::OnChangeScene, CreateScene<TestScene>()); ¿¹½Ã
	
	return (m_spScene->Update(deltaSeconds));
}

void SceneManager::Cleanup()
{
	m_spScene->Cleanup();
}

void SceneManager::RegisterActorUpdateOrderToCurrentScene(const Actor* pActor, EUpdateOrder updateOrder)
{
	
}

void SceneManager::OnChangeScene(const ScenePtr& spNextScene)
{
	Cleanup();
	m_spScene = spNextScene;
}