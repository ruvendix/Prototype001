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

	void ChangeScene(SceneBasePtr spNewScene);
	void AddRemoveDeactivatedActor(std::vector<Actor*>& outActors, Actor* pTargetActor);
	void RemoveDeactivatedAtors();

	SceneBasePtr GetCurrentScene() const { return m_spCurrentScene; }

private:
	SceneBasePtr m_spCurrentScene;
	std::vector<Actor*> m_willRemoveDeactivatedActors;
};