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
	return (m_spScene->Update(deltaSeconds));
}

void SceneManager::Cleanup()
{
	m_spScene->Cleanup();
}

void SceneManager::OnChangeScene(const ScenePtr& spNextScene)
{
	Cleanup();
	m_spScene = spNextScene;
}