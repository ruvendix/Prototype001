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

	virtual bool CheckCanMoveToCellPosition(const Position2d& destCellPos) const;

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

	template <typename TActor> // 씬 안에 액터 생성 (호출한 씬에게 자동 소속됨)
	std::shared_ptr<TActor> CreateActorToScene(EActorLayerType actorLayer, EActorUpdateOrder actorUpdateOrder = EActorUpdateOrder::Default)
	{
		const std::shared_ptr<TActor>& spNewActor = Scene::CreateActor<TActor>(actorLayer, actorUpdateOrder);
		return CreateActorToSceneImpl(spNewActor, actorLayer);
	}

	template <typename TActor> // 씬 안에 액터 생성 (호출한 씬에게 자동 소속됨)
	std::shared_ptr<TActor> CreateCloneActorToScene(const std::shared_ptr<TActor>& spSrcActor)
	{
		std::shared_ptr<TActor> spCloneActor = std::dynamic_pointer_cast<TActor>(spSrcActor->CreateClone());
		ASSERT_LOG(spCloneActor != nullptr);
		return CreateActorToSceneImpl(spCloneActor, spSrcActor->GetActorLayer());
	}

	template <typename TActor> // 씬 안에 액터 생성 (호출한 씬에게 자동 소속됨)
	std::shared_ptr<TActor> CreateActorToSceneImpl(const std::shared_ptr<TActor>& spNewActor, EActorLayerType actorLayer)
	{
		Actors& refActors = FindActors(actorLayer);

		// 중복된 액터인지 확인
		auto foundIter = std::find_if(refActors.begin(), refActors.end(),
			[&] (const ActorPtr& spOther)
			{
				return (spNewActor == spOther);
			});

		// 없을 때만 추가
		if (foundIter == refActors.cend())
		{
			refActors.push_back(spNewActor);

			// 업데이트 순서에 따른 액터 목록 처리
			m_actorsByUpdateOrder.push_back(spNewActor);
			std::stable_sort(m_actorsByUpdateOrder.begin(), m_actorsByUpdateOrder.end(),
				[&](const ActorPtr& spLhs, const ActorPtr& spRhs)
				{
					return (spLhs->GetActorUpdateOrder() < spRhs->GetActorUpdateOrder());
				});
		}

		return spNewActor;
	}

	template <typename TActor>
	void FindDerivedActors(EActorLayerType actorLayer, std::vector<std::shared_ptr<TActor>>& outActorPtrs) const
	{
		const Actors& foundActorPtrs = FindConstActors(actorLayer);
		if (foundActorPtrs.empty() == true)
		{
			return;
		}

		for (const ActorPtr& spActor : foundActorPtrs)
		{
			if (spActor == nullptr)
			{
				continue;
			}

			const std::shared_ptr<TActor>& spExactTypeActor = std::dynamic_pointer_cast<TActor>(spActor);
			if (spExactTypeActor == nullptr)
			{
				continue;
			}

			outActorPtrs.push_back(spExactTypeActor);
		}
	}

	template <typename TActor>
	std::shared_ptr<TActor> FindAnyExactTypeCellActor(EActorLayerType actorLayer, const Position2d& cellPos) const
	{
		static_assert(std::is_base_of_v<CellActor, TActor> == true, "TActor isn't derived cellActor");

		const ActorPtr& spFoundCellActor = FindAnyCellActor(actorLayer, cellPos);
		const std::shared_ptr<TActor>& spResultActor = std::dynamic_pointer_cast<TActor>(spFoundCellActor);
		if (spResultActor == nullptr)
		{
			return nullptr;
		}

		return spFoundCellActor;
	}

public:
	void RegisterMainCameraActorToScene(const ActorPtr& spMainCameraTargetActor);
	
	Actors& FindActors(EActorLayerType actorLayer);
	const Actors& FindConstActors(EActorLayerType actorLayer) const;
	ActorPtr FindAnyCellActor(EActorLayerType actorLayer, const Position2d& cellPos) const;

	void ReserveCreateEffect(const EffectSpawnInfo& effectSpawnInfo);
	void ReserveEraseActor(const std::shared_ptr<EnableSharedClass>& spActor);

	LayerActors& GetLayerActors() { return m_layerActors; }
	const LayerActors& GetLayerActors() const { return m_layerActors; }

private:
	void EraseReservedActors();
	void OnCreateEffect(const EffectSpawnInfo& effectSpawnInfo);

private:
	std::shared_ptr<CameraActor> m_spMainCameraActor = nullptr;
	Event<const EffectSpawnInfo& /* spNextScene */> m_sceneCreateEffect;

	LayerActors m_layerActors; // 씬에 있는 모든 액터 (업데이트와 렌더링은 따로 관리)
	Actors m_actorsByUpdateOrder; // 업데이트 순서에 따른 씬에 있는 모든 액터

	Actors m_reserveEraseActorsForNextFrame; // 다음 프레임에서 제거될 액터들
};