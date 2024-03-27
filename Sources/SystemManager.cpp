/*
<내용 요약>
시스템들의 관리자.
초기화와 정리의 순서를 편하게 설정할 수 있음.
*/

#include "Pch.h"
#include "SystemManager.h"

#include "FrameSystem.h"
#include "InputSystem.h"
#include "PathSystem.h"
#include "ResourceSystem.h"
#include "SceneSystem.h"
#include "CollisionSystem.h"
#include "UiSystem.h"

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

void SystemManager::Startup()
{
	AddSystem<FrameSystem>(); // 이게 무조건 처음
	AddSystem<InputSystem>();
	AddSystem<PathSystem>();
	AddSystem<ResourceSystem>();
	AddSystem<UiSystem>();
	AddSystem<SceneSystem>();
	AddSystem<CollisionSystem>();
}

bool SystemManager::Update()
{
	for (SystemBase*& pRefSystem : m_systems)
	{
		pRefSystem->Update();
	}

	return true;
}

void SystemManager::Render()
{
	GET_SYSTEM(SceneSystem)->Render();
	GET_SYSTEM(CollisionSystem)->Render();
	GET_SYSTEM(UiSystem)->Render();
}

void SystemManager::Cleanup()
{
	for (SystemBase*& pRefSystem : m_systems)
	{
		pRefSystem->Cleanup();
		SAFE_DELETE(pRefSystem);
	}
}