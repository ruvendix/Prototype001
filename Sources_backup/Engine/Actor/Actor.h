// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "ActorEnum.h"

class Actor : public IGameLoop
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
			// 에러 넣기
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

	// 자주 사용해서 따로 빼놓음
	TransformComponent* BringTransformComponent();
	const TransformComponent* BringTransformComponent() const;

	void FindRenderComponents(RenderComponentVector& outVecRenderComponent);
	
	const BitsetFlag& GetActorBitsetFlag() const { return m_actorBitsetFlag; }

private:
	std::string m_strName;
	std::unordered_map<int32, ComponentPtr> m_mapComponent;
	BitsetFlag m_actorBitsetFlag;
};