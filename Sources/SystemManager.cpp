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
	AddSystem<CollisionSystem>();
	AddSystem<SceneSystem>();
}

bool SystemManager::Update()
{
	GET_SYSTEM(FrameSystem)->Update();
	GET_SYSTEM(InputSystem)->Update();
	GET_SYSTEM(SceneSystem)->Update();
	GET_SYSTEM(UiSystem)->Update();

	return true;
}

bool SystemManager::PostUpdate()
{
	GET_SYSTEM(SceneSystem)->PostUpdate();
	GET_SYSTEM(CollisionSystem)->PostUpdate();

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
	}

	for (SystemBase*& pRefSystem : m_systems)
	{
		SAFE_DELETE(pRefSystem);
	}
}