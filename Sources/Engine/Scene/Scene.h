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
	static std::shared_ptr<TActor> CreateActor(EActorLayerType actorLayer)
	{
		std::shared_ptr<TActor> spNewActor = std::make_shared<TActor>();
		spNewActor->Startup();
		spNewActor->SetActorLayer(actorLayer);
		return (std::dynamic_pointer_cast<TActor>(spNewActor));
	}

	template <typename TActor>
	static ActorPtr CreateActorNoDynamicCast(EActorLayerType actorLayer)
	{
		const ActorPtr& spNewActor = std::make_shared<TActor>();
		spNewActor->Startup();
		spNewActor->SetActorLayer(actorLayer);
		return (spNewActor);
	}

	template <typename TActor> // 씬 안에 액터 생성 (호출한 씬에게 자동 소속됨)
	std::shared_ptr<TActor> CreateActorToScene(EActorLayerType actorLayer, EUpdateOrder updateOrder = EUpdateOrder::Default)
	{
		const std::shared_ptr<TActor>& spNewActor = Scene::CreateActor<TActor>(actorLayer);

		Actors& refActorPtrs = FindActors(actorLayer);
		if (refActorPtrs.empty() == true)
		{
			Actors newActorPtrs;
			newActorPtrs.push_back(spNewActor);

			auto insertedIter = m_mapActorPtrsStorage.insert(std::make_pair(actorLayer, newActorPtrs));
			ASSERT_LOG_RETURN_VALUE((insertedIter.second == true), nullptr);
		}
		else
		{
			refActorPtrs.push_back(spNewActor);
		}

		m_arrUpdateActors[TO_NUM(updateOrder)].push_back(spNewActor);
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

	void ReserveEraseActor(const std::shared_ptr<EnableSharedClass>& spActor);

	ActorStorage& GetActorPointersStorage() { return m_mapActorPtrsStorage; }
	const ActorStorage& GetActorPointersStorage() const { return m_mapActorPtrsStorage; }

private:
	void EraseReservedActors();

private:
	std::shared_ptr<CameraActor> m_spMainCameraActor = nullptr;
	
	ActorStorage m_mapActorPtrsStorage; // 씬에 있는 모든 액터 (업데이트와 렌더링은 따로 관리)
	std::array<Actors, TO_NUM(EUpdateOrder::Count)> m_arrUpdateActors; // 업데이트 상황에 따라 분리된 액터

	Actors m_reserveEraseActorsForNextFrame; // 다음 프레임에서 제거될 액터들
};