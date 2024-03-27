#include "Pch.h"
#include "Actor.h"

#include "ComponentBase.h"
#include "TransformComponent.h"

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
	// ��� ���ʹ� Ʈ�������� ����
	ADD_COMPONENT(this, TransformComponent);
}

bool Actor::Update()
{
	for (auto iter : m_components)
	{
		ComponentBase::Ptr spComponent = iter.second;
		assert(spComponent != nullptr);
		spComponent->Update();
	}

	return true;
}

void Actor::Render()
{
	for (auto iter : m_components)
	{
		ComponentBase::Ptr spComponent = iter.second;
		assert(spComponent != nullptr);
		spComponent->Render();
	}
}

void Actor::Cleanup()
{
	for (auto iter : m_components)
	{
		ComponentBase::Ptr spComponent = iter.second;
		assert(spComponent != nullptr);
		spComponent->Cleanup();
	}
}