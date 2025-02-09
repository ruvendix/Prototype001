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
	// 기본 플래그 설정
	m_actorBitsetFlag.BitsOn(
		EActorFlag::Activation,
		EActorFlag::RenderingTarget);

	// 트랜스폼 추가
	AddComponent<TransformComponent>();
}

bool Actor::Update(float deltaSeconds)
{
	// 부모부터 처리!
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
	// 정리는 자식부터 처리!
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

		// 렌더 컴포넌트가 맞으면 캐스팅 시도
		RenderComponent* pRenderComponent = dynamic_cast<RenderComponent*>(pComponent);
		ASSERT_LOG(pRenderComponent != nullptr);

		// 설정된 레이어에 따라 컨테이너에 넣음
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