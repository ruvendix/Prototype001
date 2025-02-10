// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneTracer.h"

#include "Engine/Scene/SceneManager.h"

DEFINE_SINGLETON(SceneTracer);

void SceneTracer::Tracer()
{
	CleanupRenderableActors();

	Scene* pScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pScene != nullptr);
	for (const Actors& actors : pScene->GetLayerActors())
	{
		for (const ActorPtr& spActor : actors)
		{
			TransformComponent* pTransformComponent = spActor->FindComponent<TransformComponent>();
			ASSERT_LOG(pTransformComponent != nullptr);
			pTransformComponent->BuildWolrdMatrix();

			const Actor::ActorBitset& actorBitsetFlag = spActor->GetActorBitsetFlag();
			if (actorBitsetFlag.IsBitOn(EActorFlag::RenderingTarget) == false)
			{
				continue;
			}

			m_arrRenderableActors[TO_NUM(spActor->GetActorLayer())].push_back(spActor);
		}
	}

	// Actor를 Y값으로 정렬 (일단은 Creature만)
	std::sort(m_arrRenderableActors[TO_NUM(EActorLayerType::Creature)].begin(),
		m_arrRenderableActors[TO_NUM(EActorLayerType::Creature)].end(),
		[](const ActorPtr& spLhs, const ActorPtr& spRhs)
		{
			const TransformComponent* pLhsTransformComponent = spLhs->BringTransformComponent();
			const TransformComponent* pRhsTransformComponent = spRhs->BringTransformComponent();

			return (pLhsTransformComponent->GetPositionY() < pRhsTransformComponent->GetPositionY());
		});
}

void SceneTracer::CleanupRenderableActors()
{
	for (Actors& actors : m_arrRenderableActors)
	{
		actors.clear();
	}
}