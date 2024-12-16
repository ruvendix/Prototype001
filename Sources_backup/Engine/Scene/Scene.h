// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class Scene : public IGameLoop
{
public:
	Scene();
	virtual ~Scene();

	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;
	virtual bool PostUpdate(float deltaSeconds) override;
	virtual void Cleanup() override;

	template <typename TActor>
	TActor* CreateActor() // Add보다 의미가 큼 (무조건 새로운 액터를 생성)
	{
		std::shared_ptr<TActor> spActor = std::make_shared<TActor>();
		spActor->Startup();
		m_vecActors.push_back(spActor);

		return (spActor.get());
	}

	std::vector<ActorPtr>& GetActors() { return m_vecActors; }
	const std::vector<ActorPtr>& GetActors() const { return m_vecActors; }

private:
	// 일단 레이어 없이 테스트
	std::vector<ActorPtr> m_vecActors;
};