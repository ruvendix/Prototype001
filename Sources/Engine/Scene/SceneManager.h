// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class SceneManager : public ICoreLoop
{
	DECLARE_SINGLETON(SceneManager)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
	template <typename TScene>
	Scene* CreateCurrentScene()
	{
		m_spScene = CreateScene<TScene>();
		return GetCurrentScene();
	}

	template <typename TScene>
	ScenePtr CreateScene()
	{
		const ScenePtr& spScene = std::make_shared<TScene>();
		spScene->Startup();
		return spScene;
	}

	template <typename TScene>
	void ReserveNextScene()
	{
		const ScenePtr& spNextScene = CreateScene<TScene>();
		m_sceneChangeEvent.RegisterEventHandler(this, &SceneManager::OnChangeScene, spNextScene);
	}

public:
	Scene* GetCurrentScene() const { return (m_spScene.get()); }

private:
	void OnChangeScene(const ScenePtr& spNextScene);

private:
	ScenePtr m_spScene = nullptr;
	Event<const ScenePtr& /* spNextScene */> m_sceneChangeEvent;
};