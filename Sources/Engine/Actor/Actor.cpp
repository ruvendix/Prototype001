// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Actor.h"

Actor::Actor()
{
	m_strActorName = "NoName";
}

Actor::Actor(const std::string& strName)
{
	m_strActorName = strName;
}

Actor::Actor(const Actor& srcActor)
{
	m_strActorName = srcActor.m_strActorName;
	m_actorBitsetFlag = srcActor.m_actorBitsetFlag;
	m_actorLayer = srcActor.m_actorLayer;

	for (auto iter : srcActor.m_mapComponent)
	{
		const ComponentPtr& spCloneComponent = (iter.second)->CreateClone();
		spCloneComponent->SetOwner(this);
		m_mapComponent.insert(std::make_pair(iter.first, spCloneComponent));
	}

	for (const ActorPtr& spChild : m_vecChild)
	{
		m_vecChild.push_back(spChild->CreateClone());
	}
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

ActorPtr Actor::CreateClone()
{
	return std::make_shared<Actor>(*this);
}

void Actor::ProcessDamaged()
{

}

void Actor::AddChild(const ActorPtr& spChild)
{
	m_vecChild.push_back(spChild);
}

void Actor::ApplyScreenCenterPosition()
{
	const Size& halfResolution = SceneRenderer::I()->GetViewerHalfResoultion();
	BringTransformComponent()->SetPosition(static_cast<float>(halfResolution.width), static_cast<float>(halfResolution.height));
}

TransformComponent* Actor::BringTransformComponent()
{
	return (FindComponent<TransformComponent>());
}

const TransformComponent* Actor::BringTransformComponent() const
{
	return (FindConstComponent<TransformComponent>());
}

void Actor::FindRenderComponents(RenderComponentVector& outVecRenderComponent) const
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

		// ������ ���̾ ���� �����̳ʿ� ����
		if (global::ValidateIndexRange(m_actorLayer, EActorLayerType::Count) == false)
		{
			DETAIL_ERROR_LOG(EErrorCode::InvalidRenderingLayer);
		}

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