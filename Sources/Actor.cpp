#include "Pch.h"
#include "Actor.h"

#include "ComponentBase.h"
#include "TransformComponent.h"

Actor::Actor()
{
	
}

Actor::Actor(const std::string strTag)
{
	m_strTag = strTag;
}

Actor::~Actor()
{
	Cleanup();
}

void Actor::Startup()
{
	// 모든 액터는 트랜스폼이 있음
	ADD_COMPONENT(this, TransformComponent);

	for (Actor* pChild : m_childs)
	{
		pChild->Startup();
	}
}

bool Actor::Update()
{
	for (auto iter : m_components)
	{
		ComponentBasePtr spComponent = iter.second;
		assert(spComponent != nullptr);
		spComponent->Update();
	}

	for (Actor* pChild : m_childs)
	{
		pChild->Update();
	}

	return true;
}

bool Actor::PostUpdate()
{
	for (auto iter : m_components)
	{
		ComponentBasePtr spComponent = iter.second;
		assert(spComponent != nullptr);
		spComponent->PostUpdate();
	}

	for (Actor* pChild : m_childs)
	{
		pChild->PostUpdate();
	}

	return true;
}

void Actor::Render()
{
	for (auto iter : m_components)
	{
		ComponentBasePtr spComponent = iter.second;
		assert(spComponent != nullptr);
		spComponent->Render();
	}

	for (Actor* pChild : m_childs)
	{
		pChild->Render();
	}
}

void Actor::Cleanup()
{
	for (auto iter : m_components)
	{
		ComponentBasePtr spComponent = iter.second;
		assert(spComponent != nullptr);
		spComponent->Cleanup();
	}

	for (Actor* pChild : m_childs)
	{
		pChild->Cleanup();
		SAFE_DELETE(pChild);
	}
}

void Actor::AddChild(Actor* pActor)
{
	m_childs.emplace_back(pActor);
}