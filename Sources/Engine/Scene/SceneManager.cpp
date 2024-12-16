// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneManager.h"

DEFINE_SINGLETON(SceneManager);

void SceneManager::Startup()
{
	RegisterEventHandler<SceneChangeEvent>(&SceneManager::OnChangeScene);
}

bool SceneManager::Update(float deltaSeconds)
{
	return (m_spScene->Update(deltaSeconds));
}

bool SceneManager::PostUpdate(float deltaSeconds)
{
	return (m_spScene->PostUpdate(deltaSeconds));
}

void SceneManager::Cleanup()
{
	m_spScene->Cleanup();
}

void SceneManager::OnChangeScene(const EventArgs& eventArgs)
{
	Cleanup();
	m_spScene = std::any_cast<ScenePtr>(eventArgs[0]);
}