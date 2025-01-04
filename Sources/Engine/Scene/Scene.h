// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class Scene : public BaseElement
{
public:
	Scene();
	virtual ~Scene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	template <typename TActor>
	static std::shared_ptr<TActor> CreateActor()
	{
		std::shared_ptr<TActor> spActor = std::make_shared<TActor>();
		spActor->Startup();
		return (std::dynamic_pointer_cast<TActor>(spActor));
	}

	template <typename TActor>
	static ActorPtr CreateActorNoDynamicCast()
	{
		const ActorPtr& spActor = std::make_shared<TActor>();
		spActor->Startup();
		return (spActor);
	}

	template <typename TActor>
	std::shared_ptr<TActor> CreateActorToScene() // 씬 안에 액터 생성 (호출한 씬에게 자동 소속됨)
	{
		const std::shared_ptr<TActor>& spActor = Scene::CreateActor<TActor>();
		m_vecActors.push_back(spActor);
		return spActor;
	}

	std::vector<ActorPtr>& GetActors() { return m_vecActors; }
	const std::vector<ActorPtr>& GetActors() const { return m_vecActors; }

private:
	// 일단 레이어 없이 테스트
	std::vector<ActorPtr> m_vecActors;
};