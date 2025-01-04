// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "ActorEnum.h"

class Actor : public BaseElement
{
public:
	using BitsetFlag = EnumBitset<EActorFlag, TO_NUM(EActorFlag::Count)>;

public:
	Actor();
	Actor(const std::string& strName);
	virtual ~Actor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	template <typename TComponent>
	TComponent* FindComponent()
	{
		const auto& foundIter = m_mapComponent.find(TComponent::s_id);
		if (foundIter == m_mapComponent.cend())
		{
			return nullptr;
		}

		return dynamic_cast<TComponent*>(foundIter->second.get());
	}

	template <typename TComponent>
	const TComponent* FindComponent() const
	{
		const auto& foundIter = m_mapComponent.find(TComponent::s_id);
		if (foundIter == m_mapComponent.cend())
		{
			return nullptr;
		}

		return dynamic_cast<const TComponent*>(foundIter->second.get());
	}

	template <typename TComponent>
	TComponent* AddComponent()
	{
		TComponent* pFoundComponent = FindComponent<TComponent>();
		if (pFoundComponent != nullptr)
		{
			DETAIL_ERROR_LOG(EErrorCode::ExistedComponent);
			return nullptr;
		}

		std::shared_ptr<TComponent> spComponent = std::make_shared<TComponent>();
		spComponent->Startup();
		spComponent->SetOwner(this);

		m_mapComponent.insert(std::make_pair(TComponent::s_id, spComponent));
		return (spComponent.get());
	}

	template <typename TComponent>
	void RemoveComponent()
	{
		TComponent* pFoundComponent = FindComponent<TComponent>();
		if (pFoundComponent == nullptr)
		{
			// 에러 넣기
			return;
		}

		m_mapComponent.erase(TComponent::s_id);
	}

	template <typename TActor>
	std::shared_ptr<TActor> GetChild(int32 childIdx)
	{
		return (std::dynamic_pointer_cast<TActor>(GetChildNoCast(childIdx)));
	}

	template <typename TActor>
	std::shared_ptr<TActor> GetChild(int32 childIdx) const
	{
		return (std::dynamic_pointer_cast<TActor>(GetChildNoCast(childIdx)));
	}

	void AddChild(const ActorPtr& spChild);

	// 자주 사용해서 따로 빼놓음
	TransformComponent* BringTransformComponent();
	const TransformComponent* BringTransformComponent() const;

	void FindRenderComponents(RenderComponentVectorArray& outArrVecRenderComponent);

	void SetActorFlagBitOn(EActorFlag actorFlag) { m_actorBitsetFlag.BitOn(actorFlag); }
	void SetActorFlagBitOff(EActorFlag actorFlag) { m_actorBitsetFlag.BitOff(actorFlag); }

	bool IsActorFlagBitOn(EActorFlag actorFlag) const { return (m_actorBitsetFlag.IsBitOn(actorFlag)); }

	const BitsetFlag& GetActorBitsetFlag() const { return m_actorBitsetFlag; }
	const ActorPtr& GetChildNoCast(int32 childIdx) const { return (m_vecChild[childIdx]); }

	void SetRenderingLayer(ERenderingLayerType renderingLayer) { m_renderingLayer = renderingLayer; }

private:
	std::string m_strName;
	std::unordered_map<int32, ComponentPtr> m_mapComponent;
	BitsetFlag m_actorBitsetFlag;

	// 자식 액터는 레퍼런스 카운트로 갖고 있어야함!
	std::vector<ActorPtr> m_vecChild;

	// 렌더링되는 순서
	ERenderingLayerType m_renderingLayer = ERenderingLayerType::WorldBackground;
};