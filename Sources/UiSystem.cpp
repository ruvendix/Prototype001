#include "Pch.h"
#include "UiSystem.h"

#include "SystemManager.h"
#include "SceneSystem.h"
#include "SceneBase.h"
#include "Actor.h"
#include "CollisionComponent.h"

UiSystem::UiSystem()
{

}

UiSystem::~UiSystem()
{

}

bool UiSystem::Update()
{
	bool bRet = true;
	for (auto iter : m_uiStorage)
	{
		UiBase::Ptr spUi = iter.second;
		if (spUi == nullptr)
		{
			continue;
		}

		bRet = spUi->Update();
	}

	return bRet;
}

void UiSystem::Render()
{
	for (auto iter : m_uiStorage)
	{
		UiBase::Ptr spUi = iter.second;
		if (spUi == nullptr)
		{
			continue;
		}

		spUi->Render();
	}
}

void UiSystem::Cleanup()
{
	for (auto iter : m_uiStorage)
	{
		UiBase::Ptr spUi = iter.second;
		if (spUi == nullptr)
		{
			continue;
		}

		spUi->Cleanup();
	}
}