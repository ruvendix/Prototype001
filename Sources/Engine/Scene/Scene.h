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

	virtual bool CheckCanMoveToCellPosition(const Position2d& destCellPos, const Actor* pExcludeActor) const;

public:
	template <typename TActor>
	static std::shared_ptr<TActor> CreateActor(EActorLayerType actorLayer, EActorUpdateOrder actorUpdateOrder = EActorUpdateOrder::Default)
	{
		const ActorPtr& spNewActor = CreateActorNoDynamicCast<TActor>(actorLayer, actorUpdateOrder);
		return (std::dynamic_pointer_cast<TActor>(spNewActor));
	}

	template <typename TActor>
	static ActorPtr CreateActorNoDynamicCast(EActorLayerType actorLayer, EActorUpdateOrder actorUpdateOrder = EActorUpdateOrder::Default)
	{
		ActorPtr spNewActor = std::make_shared<TActor>();
		spNewActor->Startup();
		spNewActor->SetActorLayer(actorLayer);
		spNewActor->SetActorUpdateOrder(actorUpdateOrder);
		return (spNewActor);
	}

	template <typename TActor> // �� �ȿ� ���� ���� (ȣ���� ������ �ڵ� �Ҽӵ�)
	std::shared_ptr<TActor> CreateActorToScene(EActorLayerType actorLayer, EActorUpdateOrder actorUpdateOrder = EActorUpdateOrder::Default)
	{
		const std::shared_ptr<TActor>& spNewActor = Scene::CreateActor<TActor>(actorLayer, actorUpdateOrder);
		return CreateActorToSceneImpl(spNewActor, actorLayer);
	}

	template <typename TActor> // �� �ȿ� ���� ���� (ȣ���� ������ �ڵ� �Ҽӵ�)
	std::shared_ptr<TActor> CreateCloneActorToScene(const std::shared_ptr<TActor>& spSrcActor)
	{
		std::shared_ptr<TActor> spCloneActor = std::dynamic_pointer_cast<TActor>(spSrcActor->CreateClone());
		ASSERT_LOG(spCloneActor != nullptr);
		return CreateActorToSceneImpl(spCloneActor, spSrcActor->GetActorLayer());
	}

	template <typename TActor> // �� �ȿ� ���� ���� (ȣ���� ������ �ڵ� �Ҽӵ�)
	std::shared_ptr<TActor> CreateActorToSceneImpl(const std::shared_ptr<TActor>& spNewActor, EActorLayerType actorLayer)
	{
		// �ߺ��� �������� Ȯ��
		auto foundIter = std::find_if(m_actors.begin(), m_actors.end(),
			[&] (const ActorPtr& spOther)
			{
				return (spNewActor == spOther);
			});

		// ���� ���� �߰�
		if (foundIter == m_actors.cend())
		{
			m_actors.push_back(spNewActor);

			// ������Ʈ ������ ���� ���� ��� ó��
			std::stable_sort(m_actors.begin(), m_actors.end(),
				[&](const ActorPtr& spLhs, const ActorPtr& spRhs)
				{
					return (spLhs->GetActorUpdateOrder() < spRhs->GetActorUpdateOrder());
				});
		}

		return spNewActor;
	}

	template <typename TActor>
	void FindExactTypeActors(EActorLayerType actorLayer, std::vector<std::shared_ptr<TActor>>& outFoundExactTypeActors) const
	{
		static_assert(std::is_base_of_v<Actor, TActor> == true, "TActor isn't derived actor");

		if (m_actors.empty() == true)
		{
			return;
		}

		for (const ActorPtr& spActor : m_actors)
		{
			if ((spActor == nullptr) ||
				(spActor->GetActorLayer() != actorLayer))
			{
				continue;
			}

			if (spActor->IsActorFlagBitOn(EActorFlag::Activation) == false)
			{
				continue;
			}

			const std::shared_ptr<TActor>& spExactTypeActor = std::dynamic_pointer_cast<TActor>(spActor);
			if (spExactTypeActor == nullptr)
			{
				continue;
			}

			outFoundExactTypeActors.push_back(spExactTypeActor);
		}
	}

	template <typename TActor>
	std::shared_ptr<TActor> FindExactTypeBaseOfCellActor(EActorLayerType actorLayer, const Position2d& cellPos) const
	{
		static_assert(std::is_base_of_v<CellActor, TActor> == true, "TActor isn't derived cellActor");

		const ActorPtr& spFoundCellActor = FindCellActor(actorLayer, cellPos, nullptr);
		const std::shared_ptr<TActor>& spResultActor = std::dynamic_pointer_cast<TActor>(spFoundCellActor);
		if (spResultActor == nullptr)
		{
			return nullptr;
		}

		return spResultActor;
	}

public:
	void RegisterMainCameraActorToScene(const ActorPtr& spMainCameraTargetActor);
	
	std::shared_ptr<CellActor> FindCellActor(EActorLayerType actorLayer, const Position2d& targetCellPos, const Actor* pExcludeActor) const;

	void ReserveCreateEffectActor(const EffectSpawnInfo& effectSpawnInfo);
	void ReserveEraseActor(const ActorPtr& spActor);

	Actors& GetActors() { return m_actors; }
	const Actors& GetActors() const { return m_actors; }

private:
	void EraseReservedActors();
	void OnCreateEffectActor(const EffectSpawnInfo& effectSpawnInfo);

private:
	std::shared_ptr<CameraActor> m_spMainCameraActor = nullptr;
	Event<const EffectSpawnInfo&> m_createEffectActorEvent;

	Actors m_actors; // ���� �ִ� ��� ���� (������Ʈ�� ���ķ� ó��, �������� ���� ó��)
	Actors m_reserveEraseActorsForNextFrame; // ���� �����ӿ��� ���ŵ� ���͵�
};