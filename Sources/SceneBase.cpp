#include "Pch.h"
#include "SceneBase.h"

#include "Actor.h"
#include "SystemManager.h"
#include "SceneSystem.h"

void SceneBase::Startup()
{
	// 액터가 추가될 때 액터의 Startup()을 호출
}

bool SceneBase::Update()
{
	for (auto actors : m_actors)
	{
		for (Actor* pActor : actors)
		{
			assert(pActor != nullptr);
			pActor->Update();
		}
	}

	return true;
}

bool SceneBase::PostUpdate()
{
	for (auto actors : m_actors)
	{
		for (Actor* pActor : actors)
		{
			assert(pActor != nullptr);
			pActor->PostUpdate();
		}
	}

	return true;
}

void SceneBase::Render()
{
	for (auto actors : m_actors)
	{
		for (Actor* pActor : actors)
		{
			assert(pActor != nullptr);
			if (pActor->GetActorState() != EActorState::Deactivated)
			{
				pActor->Render();
			}
		}
	}
}

void SceneBase::Cleanup()
{
	for (auto actors : m_actors)
	{
		for (Actor* pActor : actors)
		{
			assert(pActor != nullptr);
			SAFE_DELETE(pActor);
		}
	}
}