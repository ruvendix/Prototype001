// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class SceneManager : public EventListener
{
	DECLARE_SINGLETON(SceneManager)
	DECLARE_EVENT_HANDLER(SceneManager)

public:
	template <typename TScene>
	Scene* CreateScene()
	{
		m_spScene = std::make_shared<TScene>();
		m_spScene->Startup();
		return GetCurrentScene();
	}

	// 생성과 처리는 미리해도 교체는 지연시킴
	template <typename TScene>
	Scene* ReserveChangeScene()
	{
		ScenePtr spNextScene = std::make_shared<TScene>();
		spNextScene->Startup();

		EventArgs eventArgs;
		eventArgs.push_back(spNextScene);

		EventManager::I()->TriggerEvent(EEventId::ChangeScene, eventArgs, this);
		return (spNextScene.get());
	}

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	Scene* GetCurrentScene() const { return (m_spScene.get()); }

private:
	void OnChangeScene(const EventArgs& eventArgs);

private:
	ScenePtr m_spScene = nullptr;
};
