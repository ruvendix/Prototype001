// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/Scene/SceneEnums.h"

class Actor : public EnableSharedClass
{
public:
	using ActorBitset = EnumBitset<EActorFlag, TO_NUM(EActorFlag::Count)>;

public:
	Actor();
	Actor(const std::string& strName);
	Actor(const Actor& srcActor);

	virtual ~Actor();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	virtual void SaveToFileStream(const FileStream& fileStream) const;
	virtual void LoadFromFileStream(const FileStream& fileStream);

	virtual ActorPtr CreateClone();

public:
	template <typename TComponent>
	TComponent* GetComponent()
	{
		return const_cast<TComponent*>(GetConstComponent<TComponent>());
	}

	template <typename TComponent>
	const TComponent* GetConstComponent() const
	{
		return dynamic_cast<const TComponent*>(m_vecComponent[TComponent::s_id].get());
	}

	template <typename TComponent>
	TComponent* AddComponent()
	{
		static_assert(std::is_base_of_v<Component, TComponent> == true, "TComponent isn't derived component");

		std::shared_ptr<TComponent> spComponent = std::make_shared<TComponent>();
		spComponent->Startup();
		spComponent->SetOwner(this);

		m_vecComponent[TComponent::s_id] = spComponent;
		return (spComponent.get());
	}

	template <typename TComponent>
	void RemoveComponent()
	{
		m_vecComponent[TComponent::s_id] = nullptr;
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

#pragma region 자주 사용해서 따로 만듬
	void ApplyPosition(float x, float y);
	void ApplyPosition(const Vector2d& vPos);
	const Vector2d& BringPosition() const;
#pragma endregion

	void FindRenderComponents(RenderComponentVector& outVecRenderComponent) const;

	void SetActorFlagBitOn(EActorFlag actorFlag) { m_actorBitsetFlag.BitOn(actorFlag); }
	void SetActorFlagBitOff(EActorFlag actorFlag) { m_actorBitsetFlag.BitOff(actorFlag); }

	bool IsActorFlagBitOn(EActorFlag actorFlag) const { return (m_actorBitsetFlag.IsBitOn(actorFlag)); }

	const ActorBitset& GetActorBitsetFlag() const { return m_actorBitsetFlag; }
	const ActorPtr& GetChildNoCast(int32 childIdx) const { return (m_vecChild[static_cast<uint32>(childIdx)]); }

	void SetActorLayer(EActorLayerType actorLayer) { m_actorLayer = actorLayer; }
	EActorLayerType GetActorLayer() const { return m_actorLayer; }

	void SetActorUpdateOrder(EActorUpdateOrder actorUpdateOrder) { m_actorUpdateOrder = actorUpdateOrder; }
	EActorUpdateOrder GetActorUpdateOrder() const { return m_actorUpdateOrder; }

	int32 GetChildCount() const { return (static_cast<int32>(m_vecChild.size())); }

	void SetActorName(const std::string& strActorName) { m_strActorName = strActorName; }
	const std::string& GetActorName() const { return m_strActorName; }

private:
	std::string m_strActorName;
	std::vector<ComponentPtr> m_vecComponent;

	ActorBitset m_actorBitsetFlag;
	EActorLayerType m_actorLayer = EActorLayerType::Default;
	EActorUpdateOrder m_actorUpdateOrder = EActorUpdateOrder::Default;

	// 자식 액터는 레퍼런스 카운트로 갖고 있어야함!
	Actors m_vecChild;
};