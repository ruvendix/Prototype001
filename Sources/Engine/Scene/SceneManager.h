// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class SceneManager : public ICoreLoop
{
	DECLARE_SINGLETON(SceneManager)

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

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

	void RegisterActorUpdateOrderToCurrentScene(const Actor* pActor, EUpdateOrder updateOrder);

	Scene* GetCurrentScene() const { return (m_spScene.get()); }

private:
	void OnChangeScene(const ScenePtr& spNextScene);

private:
	ScenePtr m_spScene = nullptr;
	Event<const ScenePtr& /* spNextScene */> m_sceneChangeEvent;
};