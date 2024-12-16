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
	TActor* CreateActor() // Add���� �ǹ̰� ŭ (������ ���ο� ���͸� ����)
	{
		std::shared_ptr<TActor> spActor = std::make_shared<TActor>();
		spActor->Startup();
		m_vecActors.push_back(spActor);

		return (spActor.get());
	}

	std::vector<ActorPtr>& GetActors() { return m_vecActors; }
	const std::vector<ActorPtr>& GetActors() const { return m_vecActors; }

private:
	// �ϴ� ���̾� ���� �׽�Ʈ
	std::vector<ActorPtr> m_vecActors;
};