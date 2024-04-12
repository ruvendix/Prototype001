#pragma once

#include "IGameDefault.h"
#include "ComponentBase.h"
#include "ComponentIdCounter.h"
#include "ComponentDefines.h"

enum class EActorState
{
	Activated = 0,
	Deactivated,
};

class Actor : public IGameDefault
{
public:
	DEFINE_SMART_PTR(Actor);

public:
	Actor(const std::string strTag);
	virtual ~Actor();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

public:
	template <typename TComponent, typename TComponentId>
	TComponent::Ptr AddComponent()
	{
		std::is_convertible_v<TComponent, ComponentBase>;

		typename TComponent::Ptr spExistedComponent = GetComponent<TComponent, TComponentId>();
		if (spExistedComponent != nullptr)
		{
			return spExistedComponent;
		}

		typename TComponent::Ptr spNewComponent = std::make_shared<TComponent>();
		spNewComponent->SetOwner(this);
		spNewComponent->Startup();

		m_components.emplace(TComponentId::s_id, spNewComponent);
		return spNewComponent;
	}

	template <typename TComponent, typename TComponentId>
	void RemoveComponent()
	{
		std::is_convertible_v<TComponent, ComponentBase>;

		typename TComponent::Ptr* spRemoveComponent = GetComponent<TComponent>();
		if (spRemoveComponent == nullptr)
		{
			return;
		}

		spRemoveComponent->Cleanup();
		m_components.erase(TComponentId::s_id);
	}

	template <typename TComponent, typename TComponentId>
	typename TComponent::Ptr GetComponent()
	{
		std::is_convertible_v<TComponent, ComponentBase>;

		auto foundIter = m_components.find(TComponentId::s_id);
		if (foundIter == m_components.cend())
		{
			return nullptr;
		}

		return std::dynamic_pointer_cast<TComponent>(foundIter->second);
	}

public:
	EActorState GetActorState() const { return m_actorState; }
	void        SetActorState(EActorState actorState) { m_actorState = actorState; }

private:
	std::string m_strTag;
	std::unordered_map<uint32, ComponentBase::Ptr> m_components;
	EActorState m_actorState = EActorState::Activated;
};