/*
<내용 요약>
씬을 관리하는 시스템
*/

#pragma once
#include "SystemBase.h"
#include "SceneBase.h"

class Actor;

class SceneSystem : public SystemBase
{
public:
	SceneSystem();
	virtual ~SceneSystem();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	void ChangeScene(SceneBase::Ptr spNewScene);
	void AddRemoveDeactivatedActor(std::vector<Actor*>& outActors, Actor* pTargetActor);
	void RemoveDeactivatedAtors();

	SceneBase::Ptr GetCurrentScene() const { return m_spCurrentScene; }

private:
	SceneBase::Ptr m_spCurrentScene;
	std::vector<Actor*> m_willRemoveDeactivatedActors;
};