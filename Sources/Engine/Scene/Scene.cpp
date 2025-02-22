// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Scene.h"

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
	m_createEffectActorEvent.ExcuteIfBound();

	// ������Ʈ ������� �̹� ���ĵ� ����
	for (const ActorPtr& spActor : m_actors)
	{
		if ((spActor == nullptr) ||
			(spActor->IsActorFlagBitOn(EActorFlag::Activation) == false))
		{
			continue;
		}

		if (spActor->Update(deltaSeconds) == false)
		{
			return false;
		}
	}

	return true;
}

void Scene::Cleanup()
{
	for (const ActorPtr& spActor : m_actors)
	{
		if (spActor != nullptr)
		{
			spActor->Cleanup();
		}
	}
}

bool Scene::CheckCanMoveToCellPosition(const Position2d& destCellPos, const Actor* pExcludeActor) const
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

	pCameraComponent->SetFollowTargetActor(spMainCameraTargetActor);
	SceneRenderer::I()->SetMainCameraActor(m_spMainCameraActor);
}

std::shared_ptr<CellActor> Scene::FindCellActor(EActorLayerType actorLayer, const Position2d& targetCellPos, const Actor* pExcludeActor) const
{
	std::vector<std::shared_ptr<CellActor>> foundCellActors;
	FindExactTypeActors<CellActor>(actorLayer, foundCellActors);
	if (foundCellActors.empty() == true)
	{
		return nullptr;
	}

	for (const std::shared_ptr<CellActor>& spCellActor : foundCellActors)
	{
		if (pExcludeActor == spCellActor.get())
		{
			continue;
		}

		if (spCellActor->CheckEqaulCellPosition(targetCellPos) == true)
		{
			return spCellActor;
		}

		if (spCellActor->CheckMovingState() == true)
		{
			const CellActorMoveComponent* pCellActorMoveComponent = spCellActor->FindComponent<CellActorMoveComponent>();
			if ((pCellActorMoveComponent != nullptr) &&
				(pCellActorMoveComponent->GetDestinationCellPosition() == targetCellPos))
			{
				return spCellActor;
			}
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

	for (const ActorPtr& spReserveEraseActor : m_reserveEraseActorsForNextFrame)
	{
		if (spReserveEraseActor == nullptr)
		{
			continue;
		}

		// �����ϱ� ���� ������ �̸��� ���
		const std::string& strActorName = spReserveEraseActor->GetActorName();

		auto foundEraseActorIter = std::find_if(m_actors.begin(), m_actors.end(),
			[&] (const ActorPtr& spOtherActor)
			{
				return (spReserveEraseActor == spOtherActor);
			});

		if (foundEraseActorIter != m_actors.cend())
		{
			m_actors.erase(foundEraseActorIter);
		}

		DEFAULT_TRACE_LOG("���� ����! (%s)", strActorName.c_str());
	}

	m_reserveEraseActorsForNextFrame.clear();
}

void Scene::OnCreateEffectActor(const EffectSpawnInfo& effectSpawnInfo)
{
	const std::shared_ptr<EffectActor>& spEffectActor = CreateActorToScene<EffectActor>(EActorLayerType::Effect, EActorUpdateOrder::Post);
	spEffectActor->SpawnEffect(effectSpawnInfo);
	DEFAULT_TRACE_LOG("����Ʈ ����! (%s)", effectSpawnInfo.strEffectName.c_str());
}

void Scene::ReserveCreateEffectActor(const EffectSpawnInfo& effectSpawnInfo)
{
	m_createEffectActorEvent.RegisterEventHandler(this, &Scene::OnCreateEffectActor, effectSpawnInfo);
}

void Scene::ReserveEraseActor(const ActorPtr& spActor)
{
	m_reserveEraseActorsForNextFrame.push_back(spActor);

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