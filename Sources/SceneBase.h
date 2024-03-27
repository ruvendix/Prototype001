/*
<내용 요약>
씬 기본형
*/

#pragma once
#include "IDefault.h"

class Actor;

class SceneBase : public IDefault
{
public:
	DEFINE_SMART_PTR(SceneBase);

public:
	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

public:
	template <typename TActor>
	TActor* CreateActor()
	{
		TActor* pNewActor = new TActor("Empty");
		return CreateActorImpl(pNewActor);
	}

	template <typename TActor>
	TActor* CreateActor(const std::string& strActorTag)
	{
		TActor* pNewActor = new TActor(strActorTag);
		return CreateActorImpl(pNewActor);
	}

	template <typename TActor>
	TActor* CreateActorImpl(TActor* pActor)
	{
		pActor->Startup();
		m_actors.emplace_back(pActor);
		return pActor;
	}

public:
	std::vector<Actor*>& GetActors() { return m_actors; }

private:
	std::vector<Actor*> m_actors;
};