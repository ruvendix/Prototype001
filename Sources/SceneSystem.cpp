/*
<내용 요약>
씬을 관리하는 시스템
*/

#include "Pch.h"
#include "SceneSystem.h"

#include "StartScene.h"
#include "Actor.h"

SceneSystem::SceneSystem()
{

}

SceneSystem::~SceneSystem()
{

}

void SceneSystem::Startup()
{
	ChangeScene(std::make_shared<StartScene>());
}

bool SceneSystem::Update()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->Update();
	}

	return true;
}

bool SceneSystem::PostUpdate()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->PostUpdate();
	}

	return true;
}

void SceneSystem::Render()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->Render();
	}
}

void SceneSystem::Cleanup()
{
	if (m_spCurrentScene != nullptr)
	{
		m_spCurrentScene->Cleanup();
	}
}

void SceneSystem::ChangeScene(SceneBase::Ptr spNewScene)
{
	Cleanup();
	spNewScene->Startup();

	m_spCurrentScene = spNewScene;
}

void SceneSystem::AddRemoveDeactivatedActor(std::vector<Actor*>& outActors, Actor* pTargetActor)
{
	// 삭제
	auto willRemoveActor = std::remove(outActors.begin(), outActors.end(), pTargetActor);
	outActors.erase(willRemoveActor);

	// 추가
	m_willRemoveDeactivatedActors.emplace_back(pTargetActor);
}

void SceneSystem::RemoveDeactivatedAtors()
{
	if (m_willRemoveDeactivatedActors.empty() == true)
	{
		return;
	}

	for (Actor* pActor : m_willRemoveDeactivatedActors)
	{
		SAFE_DELETE(pActor);
	}
	m_willRemoveDeactivatedActors.clear();
}