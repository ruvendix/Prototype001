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
	// ������ ���Ͱ� �ִٸ� ���⼭ ����
	EraseReservedActors();

	// ����Ʈ ����
	m_sceneCreateEffect.ExcuteIfBound();

	// ������Ʈ�� ���� �����ϴ� ���� ������� ó��
	for (const ActorPtr& spActorByUpdateOrder : m_actorsByUpdateOrder)
	{
		if ((spActorByUpdateOrder != nullptr) &&
			(spActorByUpdateOrder->Update(deltaSeconds) == false))
		{
			return false;
		}
	}

	// ���� ������Ʈ�� 2���� �Ǵ� ���� ����
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

	// ���࿡ Ÿ�� ���Ͱ� ���ʶ�� ����
	if (spMainCameraTargetActor.get() == pCameraComponent->GetOwner())
	{
		TRACE_LOG(LogDefault, "Ÿ�� ���Ͱ� ���� ī�޶��� ���ʶ� ���� �Ұ���!");
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

#pragma region ���̾� ���� ó��
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

#pragma region ������Ʈ ���� ó��
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

		DEFAULT_TRACE_LOG("���� ����! (%s)", strActorName.c_str());
	}

	m_reserveEraseActorsForNextFrame.clear();
}

void Scene::OnCreateEffect(const EffectSpawnInfo& effectSpawnInfo)
{
	const std::shared_ptr<EffectActor>& spEffectActor = CreateActorToScene<EffectActor>(EActorLayerType::Effect, EActorUpdateOrder::Post);
	spEffectActor->SpawnEffect(effectSpawnInfo);
	DEFAULT_TRACE_LOG("����Ʈ ����! (%s)", effectSpawnInfo.strEffectName.c_str());
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

	// �ߺ��� �ִٸ� �ߺ� ����
	if (overlapIter != m_reserveEraseActorsForNextFrame.cend())
	{
		m_reserveEraseActorsForNextFrame.erase(overlapIter, m_reserveEraseActorsForNextFrame.end());
	}
}