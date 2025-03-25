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
	//m_strActorName = srcActor.m_strActorName;
	m_actorBitsetFlag = srcActor.m_actorBitsetFlag;
	m_actorLayer = srcActor.m_actorLayer;

	// 복사 생성자라서 미리 할당
	uint32 componentCount = GET_COMPILEITME_ID_COUNT(ComponentIdCounter);
	m_vecComponent.resize(componentCount);

	for (uint32 i = 0; i < componentCount; ++i)
	{
		const ComponentPtr& spSrcComponent = srcActor.m_vecComponent[i];
		if (spSrcComponent == nullptr)
		{
			continue;
		}

		const ComponentPtr& spCloneComponent = spSrcComponent->CreateClone();
		spCloneComponent->SetOwner(this);
		m_vecComponent[i] = spCloneComponent;
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
	if (m_vecComponent.empty() == true)
	{
		m_vecComponent.resize(GET_COMPILEITME_ID_COUNT(ComponentIdCounter));
	}

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
	for (const ComponentPtr& spComponent : m_vecComponent)
	{
		if (spComponent != nullptr)
		{
			spComponent->Update(deltaSeconds);
		}
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

	for (const ComponentPtr& spComponent : m_vecComponent)
	{
		if (spComponent != nullptr)
		{
			spComponent->Cleanup();
		}
	}
	m_vecComponent.clear();
}

void Actor::SaveToFileStream(const FileStream& fileStream) const
{
	if (fileStream.IsOpenFileStream() == false)
	{
		DETAIL_ERROR_LOG(EngineErrorHandler, EEngineErrorCode::InvalidFileStream);
	}
}

void Actor::LoadFromFileStream(const FileStream& fileStream)
{
	if (fileStream.IsOpenFileStream() == false)
	{
		DETAIL_ERROR_LOG(EngineErrorHandler,EEngineErrorCode::InvalidFileStream);
	}
}

ActorPtr Actor::CreateClone()
{
	return std::make_shared<Actor>(*this);
}

void Actor::AddChild(const ActorPtr& spChild)
{
	m_vecChild.push_back(spChild);
}

void Actor::ApplyScreenCenterPosition()
{
	const Size& halfResolution = SceneRenderer::I()->GetViewerHalfResoultion();
	ApplyPosition(static_cast<float>(halfResolution.width), static_cast<float>(halfResolution.height));
}

void Actor::ApplyPosition(float x, float y)
{
	Vector2d vPos(x, y);
	ApplyPosition(vPos);
}

void Actor::ApplyPosition(const Vector2d& vPos)
{
	TransformComponent* pTransformComponent = GetComponent<TransformComponent>();
	ASSERT_LOG_RETURN(pTransformComponent != nullptr);
	pTransformComponent->SetPosition(vPos);
}

const Vector2d& Actor::BringPosition() const
{
	const TransformComponent* pTransformComponent = GetConstComponent<TransformComponent>();
	ASSERT_LOG(pTransformComponent != nullptr);
	return (pTransformComponent->GetPosition());
}

void Actor::FindRenderComponents(RenderComponentVector& outVecRenderComponent) const
{
	for (const ComponentPtr& spComponent : m_vecComponent)
	{
		if (spComponent == nullptr)
		{
			continue;
		}

		Component* pComponent = spComponent.get();
		if ((pComponent == nullptr) ||
			(pComponent->IsRenderComponent() == false))
		{
			continue;
		}

		// 렌더 컴포넌트가 맞으면 캐스팅 시도
		RenderComponent* pRenderComponent = dynamic_cast<RenderComponent*>(pComponent);
		ASSERT_LOG(pRenderComponent != nullptr);

		// 설정된 레이어에 따라 컨테이너에 넣음
		if (global::ValidateIndexRangeByEnum(m_actorLayer, EActorLayerType::Count) == false)
		{
			DETAIL_ERROR_LOG(EngineErrorHandler, EEngineErrorCode::InvalidRenderingLayer);
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