// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Scene.h"

namespace
{
	Actors g_nullActorPtrs;
}

Scene::Scene()
{

}

Scene::~Scene()
{
	Cleanup();
}

void Scene::Startup()
{

}

bool Scene::Update(float deltaSeconds)
{
	// 제거할 액터가 있다면 여기서 제거
	EraseReservedActors();

	// 이펙트 생성
	m_sceneCreateEffect.ExcuteIfBound();

	// 업데이트는 따로 관리하는 액터 목록으로 처리
	for (const ActorPtr& spActorByUpdateOrder : m_actorsByUpdateOrder)
	{
		if ((spActorByUpdateOrder != nullptr) &&
			(spActorByUpdateOrder->Update(deltaSeconds) == false))
		{
			return false;
		}
	}

	// 씬은 업데이트가 2종류 또는 여러 종류
	return true;
}

void Scene::Cleanup()
{
	for (const Actors& actors : m_layerActors)
	{
		for (const ActorPtr& spActor : actors)
		{
			spActor->Cleanup();
		}
	}
}

bool Scene::CheckCanMoveToCellPosition(const Position2d& destCellPos) const
{
	return true;
}

void Scene::RegisterMainCameraActorToScene(const ActorPtr& spMainCameraTargetActor)
{
	m_spMainCameraActor = CreateActorToScene<CameraActor>(EActorLayerType::Default, EActorUpdateOrder::Post);
	
	CameraComponent* pCameraComponent = m_spMainCameraActor->FindComponent<CameraComponent>();
	ASSERT_LOG_RETURN(pCameraComponent != nullptr);

	// 만약에 타겟 액터가 오너라면 무시
	if (spMainCameraTargetActor.get() == pCameraComponent->GetOwner())
	{
		TRACE_LOG(LogDefault, "타겟 액터가 메인 카메라의 오너라서 설정 불가능!");
		return;
	}

	pCameraComponent->SetTargetActor(spMainCameraTargetActor);
	SceneRenderer::I()->SetMainCameraActor(m_spMainCameraActor);
}

Actors& Scene::FindActors(EActorLayerType actorLayer)
{
	return const_cast<Actors&>(FindConstActors(actorLayer));
}

const Actors& Scene::FindConstActors(EActorLayerType actorLayer) const
{
	return (m_layerActors[TO_NUM(actorLayer)]);
}

ActorPtr Scene::FindAnyCellActor(EActorLayerType actorLayer, const Position2d& cellPos) const
{
	std::vector<std::shared_ptr<CellActor>> foundActorPtrs;
	FindDerivedActors<CellActor>(actorLayer, foundActorPtrs);
	if (foundActorPtrs.empty() == true)
	{
		return nullptr;
	}

	for (const std::shared_ptr<CellActor>& spCellActor : foundActorPtrs)
	{
		if (spCellActor->CheckEqaulCellPosition(cellPos) == true)
		{
			return spCellActor;
		}
	}

	return nullptr;
}

void Scene::EraseReservedActors()
{
	if (m_reserveEraseActorsForNextFrame.empty() == true)
	{
		return;
	}

	for (const ActorPtr& spActor : m_reserveEraseActorsForNextFrame)
	{
		if (spActor == nullptr)
		{
			continue;
		}

		const std::string& strActorName = spActor->GetActorName();

#pragma region 레이어 액터 처리
		Actors& refLayerActors = FindActors(spActor->GetActorLayer());
		auto foundLayerActorIter = std::find_if(refLayerActors.begin(), refLayerActors.end(),
			[&] (const ActorPtr& spOtherActor)
			{
				return (spActor == spOtherActor);
			});

		if (foundLayerActorIter != refLayerActors.cend())
		{
			refLayerActors.erase(foundLayerActorIter);
		}
#pragma endregion

#pragma region 업데이트 액터 처리
		auto foundActorByUpdateOrderIter = std::find_if(m_actorsByUpdateOrder.begin(), m_actorsByUpdateOrder.end(),
			[&](const ActorPtr& spOtherActor)
			{
				return (spActor == spOtherActor);
			});

		if (foundActorByUpdateOrderIter != m_actorsByUpdateOrder.cend())
		{
			m_actorsByUpdateOrder.erase(foundActorByUpdateOrderIter);
		}
#pragma endregion

		DEFAULT_TRACE_LOG("액터 제거! (%s)", strActorName.c_str());
	}

	m_reserveEraseActorsForNextFrame.clear();
}

void Scene::OnCreateEffect(const EffectSpawnInfo& effectSpawnInfo)
{
	const std::shared_ptr<EffectActor>& spEffectActor = CreateActorToScene<EffectActor>(EActorLayerType::Effect, EActorUpdateOrder::Post);
	spEffectActor->SpawnEffect(effectSpawnInfo);
	DEFAULT_TRACE_LOG("이펙트 생성! (%s)", effectSpawnInfo.strEffectName.c_str());
}

void Scene::ReserveCreateEffect(const EffectSpawnInfo& effectSpawnInfo)
{
	m_sceneCreateEffect.RegisterEventHandler(this, &Scene::OnCreateEffect, effectSpawnInfo);
}

void Scene::ReserveEraseActor(const std::shared_ptr<EnableSharedClass>& spActor)
{
	m_reserveEraseActorsForNextFrame.push_back(std::static_pointer_cast<Actor>(spActor));

	auto overlapIter = std::unique(m_reserveEraseActorsForNextFrame.begin(), m_reserveEraseActorsForNextFrame.end(),
		[](const ActorPtr& spLhsActor, const ActorPtr& spRhsActor)
		{
			return (spLhsActor == spRhsActor);
		});

	// 중복이 있다면 중복 제거
	if (overlapIter != m_reserveEraseActorsForNextFrame.cend())
	{
		m_reserveEraseActorsForNextFrame.erase(overlapIter, m_reserveEraseActorsForNextFrame.end());
	}
}