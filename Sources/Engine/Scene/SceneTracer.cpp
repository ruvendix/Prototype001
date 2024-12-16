// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneTracer.h"

#include "Engine/Scene/SceneManager.h"

DEFINE_SINGLETON(SceneTracer);

void SceneTracer::Tracer()
{
	m_vecRenderingTargetActor.clear();

	Scene* pScene = SceneManager::I()->GetCurrentScene();
	for (ActorPtr& spActor : pScene->GetActors())
	{
		TransformComponent* pTransformComponent = spActor->FindComponent<TransformComponent>();
		ASSERT_LOG(pTransformComponent != nullptr);
		pTransformComponent->BuildWolrdMatrix();

		const Actor::BitsetFlag& actorBitsetFlag = spActor->GetActorBitsetFlag();
		if (actorBitsetFlag.IsBitOn(EActorFlag::RenderingTarget) == false)
		{
			continue;
		}

		m_vecRenderingTargetActor.push_back(spActor);
	}
}