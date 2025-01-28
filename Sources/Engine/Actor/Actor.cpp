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
	m_actorBitsetFlag.BitsOn(
		EActorFlag::Activation,
		EActorFlag::RenderingTarget);

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

void Actor::SaveToFileStream(const FileStream& fileStream) const
{
	if (fileStream.IsOpenFileStream() == false)
	{
		DETAIL_ERROR_LOG(EErrorCode::InvalidFileStream);
	}
}

void Actor::LoadFromFileStream(const FileStream& fileStream)
{
	if (fileStream.IsOpenFileStream() == false)
	{
		DETAIL_ERROR_LOG(EErrorCode::InvalidFileStream);
	}
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

void Actor::FindRenderComponents(RenderComponentVectorArray& outArrVecRenderComponent)
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

		// ������ ������ ���̾ ���� �����̳ʿ� ����
		int32 renderingLayer = TO_NUM(m_renderingLayer);
		if (global::ValidateIndexRange(renderingLayer, TO_NUM(ERenderingLayerType::Count)) == false)
		{
			DETAIL_ERROR_LOG(EErrorCode::InvalidRenderingLayer);
		}

		outArrVecRenderComponent[renderingLayer].push_back(pRenderComponent);
	}

	for (auto& spChild : m_vecChild)
	{
		if (spChild->IsActorFlagBitOn(EActorFlag::RenderingTarget))
		{
			spChild->FindRenderComponents(outArrVecRenderComponent);
		}
	}
}