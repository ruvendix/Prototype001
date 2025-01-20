// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class Scene : public ICoreLoop
{
public:
	Scene();
	virtual ~Scene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual void Cleanup() override;

public:
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
	std::shared_ptr<TActor> CreateActorToScene(EUpdateOrder updateOrder = EUpdateOrder::Default) // 씬 안에 액터 생성 (호출한 씬에게 자동 소속됨)
	{
		const std::shared_ptr<TActor>& spActor = Scene::CreateActor<TActor>();
		m_vecActors.push_back(spActor);
		m_arrUpdateActors[TO_NUM(updateOrder)].push_back(spActor);
		return spActor;
	}

public:
	void RegisterMainCameraActorToScene(const ActorPtr& spMainCameraTargetActor);

	std::vector<ActorPtr>& GetActors() { return m_vecActors; }
	const std::vector<ActorPtr>& GetActors() const { return m_vecActors; }

private:
	std::shared_ptr<CameraActor> m_spMainCameraActor = nullptr;
	std::vector<ActorPtr> m_vecActors; // 씬에 있는 모든 액터
	std::array<std::vector<ActorPtr>, TO_NUM(EUpdateOrder::Count)> m_arrUpdateActors; // 업데이트 상황에 따라 분리된 액터
};