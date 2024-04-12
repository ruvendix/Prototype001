#pragma once

#include "IGameDefault.h"

enum class EActorLayer
{
	World = 0,
	Character,
	Count,
};

class Actor;

class SceneBase : public IGameDefault
{
public:
	using Actors = std::array<std::vector<Actor*>, ENUM_TO_NUM(EActorLayer::Count)>;

public:
	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

public:
	template <typename TActor>
	TActor* CreateActor(EActorLayer actorLayer)
	{
		TActor* pNewActor = new TActor("Empty");
		return CreateActorImpl(pNewActor, actorLayer);
	}

	template <typename TActor>
	TActor* CreateActor(const std::string& strActorTag, EActorLayer actorLayer)
	{
		TActor* pNewActor = new TActor(strActorTag);
		return CreateActorImpl(pNewActor, actorLayer);
	}

	template <typename TActor>
	TActor* CreateActorImpl(TActor* pActor, EActorLayer actorLayer)
	{
		if (actorLayer == EActorLayer::Count)
		{
			return nullptr;
		}

		pActor->Startup();
		m_actors[ENUM_TO_NUM(actorLayer)].emplace_back(pActor);
		return pActor;
	}

public:
	Actors& GetActors() { return m_actors; }

private:
	Actors m_actors;
};