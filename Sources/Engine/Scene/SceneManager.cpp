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
	// 메인 카메라를 항상 첫번째로 업데이트!
	const std::shared_ptr<CameraActor>& spMainCameraActor = SceneRenderer::I()->GetMainCameraActor();
	spMainCameraActor->PostUpdate(deltaSeconds);

	// 이후에는 씬에 소속된 액터들을 업데이트!
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