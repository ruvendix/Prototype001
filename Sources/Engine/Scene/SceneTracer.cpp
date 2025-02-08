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
	for (auto iter : pScene->GetActorPointersStorage())
	{
		const Actors& actorPtrs = (iter.second);
		for (const ActorPtr& spActor : actorPtrs)
		{
			TransformComponent* pTransformComponent = spActor->FindComponent<TransformComponent>();
			ASSERT_LOG(pTransformComponent != nullptr);
			pTransformComponent->BuildWolrdMatrix();

			const Actor::ActorBitset& actorBitsetFlag = spActor->GetActorBitsetFlag();
			if (actorBitsetFlag.IsBitOn(EActorFlag::RenderingTarget) == false)
			{
				continue;
			}

			m_arrRenderableActorPtrs[TO_NUM(spActor->GetActorLayer())].push_back(spActor);
		}
	}

	// Actor를 Y값으로 정렬 (일단은 Creature만)
	std::sort(m_arrRenderableActorPtrs[TO_NUM(EActorLayerType::Creature)].begin(),
		m_arrRenderableActorPtrs[TO_NUM(EActorLayerType::Creature)].end(),
		[] (const ActorPtr& spLhs, const ActorPtr& spRhs)
		{
			const TransformComponent* pLhsTransformComponent = spLhs->BringTransformComponent();
			const TransformComponent* pRhsTransformComponent = spRhs->BringTransformComponent();

			return (pLhsTransformComponent->GetPositionY() < pRhsTransformComponent->GetPositionY());
		});
}

void SceneTracer::CleanupRenderableActors()
{
	for (Actors& actors : m_arrRenderableActorPtrs)
	{
		actors.clear();
	}
}