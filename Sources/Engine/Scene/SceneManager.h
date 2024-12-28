// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "SceneEvent.h"

class SceneManager : public IGameLoop
{
	DECLARE_SINGLETON(SceneManager)
	DEFINE_EVENT_HANDLER;

public:
	template <typename TScene>
	Scene* CreateScene()
	{
		m_spScene = std::make_shared<TScene>();
		m_spScene->Startup();
		return GetCurrentScene();
	}

	// 씬을 교체할 때는 안전하게 지연시켜서 처리함
	template <typename TScene>
	void ReserveChangeScene()
	{
		ScenePtr spNextScene = std::make_shared<TScene>(this);
		ReserveEvent<SceneChangeEvent>(spNextScene);
	}

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	Scene* GetCurrentScene() const { return (m_spScene.get()); }

private:
	void OnChangeScene(const CallbackArgs& eventArgs);

private:
	ScenePtr m_spScene = nullptr;
};
