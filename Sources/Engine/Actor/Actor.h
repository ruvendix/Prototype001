// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "ActorEnums.h"
#include "Engine/Scene/SceneEnums.h"

class Actor : public ICoreLoop
{
public:
	using ActorBitset = EnumBitset<EActorFlag, TO_NUM(EActorFlag::Count)>;

public:
	Actor();
	Actor(const std::string& strName);
	virtual ~Actor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void SaveToFileStream(const FileStream& fileStream) const;
	virtual void LoadFromFileStream(const FileStream& fileStream);

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

		const auto& retInsert = m_mapComponent.insert(std::make_pair(TComponent::s_id, spComponent));
		if (retInsert.second == false)
		{
			return nullptr;
		}

		return (spComponent.get());
	}

	template <typename TComponent>
	void RemoveComponent()
	{
		TComponent* pFoundComponent = FindComponent<TComponent>();
		if (pFoundComponent == nullptr)
		{
			// ���� �ֱ�
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

public:
	void AddChild(const ActorPtr& spChild);
	void ApplyScreenCenterPosition();

	// ���� ����ؼ� ���� ������
	TransformComponent* BringTransformComponent();
	const TransformComponent* BringTransformComponent() const;

	void FindRenderComponents(RenderComponentVectorArray& outArrVecRenderComponent);

	void SetActorFlagBitOn(EActorFlag actorFlag) { m_actorBitsetFlag.BitOn(actorFlag); }
	void SetActorFlagBitOff(EActorFlag actorFlag) { m_actorBitsetFlag.BitOff(actorFlag); }

	bool IsActorFlagBitOn(EActorFlag actorFlag) const { return (m_actorBitsetFlag.IsBitOn(actorFlag)); }

	const ActorBitset& GetActorBitsetFlag() const { return m_actorBitsetFlag; }
	const ActorPtr& GetChildNoCast(int32 childIdx) const { return (m_vecChild[childIdx]); }

	void SetRenderingLayer(ERenderingLayerType renderingLayer) { m_renderingLayer = renderingLayer; }

	int32 GetChildCount() const { return (static_cast<int32>(m_vecChild.size())); }

private:
	std::string m_strName;
	std::unordered_map<int32, ComponentPtr> m_mapComponent;
	ActorBitset m_actorBitsetFlag;

	// �ڽ� ���ʹ� ���۷��� ī��Ʈ�� ���� �־����!
	std::vector<ActorPtr> m_vecChild;

	// �������Ǵ� ����
	ERenderingLayerType m_renderingLayer = ERenderingLayerType::WorldBackground;
};