// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Actor.h"

Actor::Actor()
{
	m_strName = "NoName";
}

Actor::Actor(const std::string& strName)
{
	m_strName = strName;
}

Actor::~Actor()
{
	Cleanup();
}

void Actor::Startup()
{
	// �⺻ �÷��� ����
	m_actorBitsetFlag.BitsOn({
			EActorFlag::Activation,
			EActorFlag::RenderingTarget});

	// Ʈ������ �߰�
	AddComponent<TransformComponent>();
}

bool Actor::Update(float deltaSeconds)
{
	// �θ���� ó��!
	for (auto& iter : m_mapComponent)
	{
		(iter.second)->Update(deltaSeconds);
	}

	for (auto& spChild : m_vecChild)
	{
		spChild->Update(deltaSeconds);
	}

	return true;
}

bool Actor::PostUpdate(float deltaSeconds)
{
	for (auto& iter : m_mapComponent)
	{
		(iter.second)->PostUpdate(deltaSeconds);
	}

	for (auto& spChild : m_vecChild)
	{
		spChild->PostUpdate(deltaSeconds);
	}

	return true;
}

void Actor::Cleanup()
{
	// ������ �ڽĺ��� ó��!
	for (auto& spChild : m_vecChild)
	{
		spChild->Cleanup();
	}

	for (auto& iter : m_mapComponent)
	{
		(iter.second)->Cleanup();
	}

	m_mapComponent.clear();
}

void Actor::AddChild(const ActorPtr& spChild)
{
	m_vecChild.push_back(spChild);
}

TransformComponent* Actor::BringTransformComponent()
{
	return (FindComponent<TransformComponent>());
}

const TransformComponent* Actor::BringTransformComponent() const
{
	return (FindComponent<TransformComponent>());
}

void Actor::FindRenderComponents(RenderComponentVector& outVecRenderComponent)
{
	for (auto& iter : m_mapComponent)
	{
		Component* pComponent = (iter.second).get();
		if ((pComponent == nullptr) ||
			(pComponent->IsRenderComponent() == false))
		{
			continue;
		}

		// ���� ������Ʈ�� ������ ĳ���� �õ�
		RenderComponent* pRenderComponent = dynamic_cast<RenderComponent*>(pComponent);
		ASSERT_LOG(pRenderComponent != nullptr);
		outVecRenderComponent.push_back(pRenderComponent);
	}

	for (auto& spChild : m_vecChild)
	{
		if (spChild->IsActorFlagBitOn(EActorFlag::RenderingTarget))
		{
			spChild->FindRenderComponents(outVecRenderComponent);
		}
	}
}