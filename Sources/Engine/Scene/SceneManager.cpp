// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneManager.h"

#include "Client/Scene/TestScene.h"

DEFINE_SINGLETON(SceneManager);

void SceneManager::Startup()
{

}

bool SceneManager::Update(float deltaSeconds)
{
	m_sceneChangeEvent.ExcuteIfBound();
	// m_sceneChangeEvent.RegisterEventHandler(this, &SceneManager::OnChangeScene, CreateScene<TestScene>()); 예시
	
	return (m_spScene->Update(deltaSeconds));
}

bool SceneManager::PostUpdate(float deltaSeconds)
{
	// 메인 카메라를 항상 첫번째로 업데이트!
	const std::shared_ptr<CameraActor>& spMainCameraActor = SceneRenderer::I()->GetMainCameraActor();
	if (spMainCameraActor != nullptr)
	{
		spMainCameraActor->PostUpdate(deltaSeconds);
	}

	// 이후에는 씬에 소속된 액터들을 업데이트!
	return (m_spScene->PostUpdate(deltaSeconds));
}

void SceneManager::Cleanup()
{
	m_spScene->Cleanup();
}

void SceneManager::OnChangeScene(const ScenePtr& spNextScene)
{
	Cleanup();
	m_spScene = spNextScene;

	SceneRenderer::I()->SetMainCameraActor(nullptr);
}