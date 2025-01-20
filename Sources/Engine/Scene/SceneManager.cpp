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
	// m_sceneChangeEvent.RegisterEventHandler(this, &SceneManager::OnChangeScene, CreateScene<TestScene>()); ����
	
	return (m_spScene->Update(deltaSeconds));
}

bool SceneManager::PostUpdate(float deltaSeconds)
{
	// ���� ī�޶� �׻� ù��°�� ������Ʈ!
	const std::shared_ptr<CameraActor>& spMainCameraActor = SceneRenderer::I()->GetMainCameraActor();
	if (spMainCameraActor != nullptr)
	{
		spMainCameraActor->PostUpdate(deltaSeconds);
	}

	// ���Ŀ��� ���� �Ҽӵ� ���͵��� ������Ʈ!
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