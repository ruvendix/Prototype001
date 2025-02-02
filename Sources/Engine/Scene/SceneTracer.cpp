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

		const Actor::ActorBitset& actorBitsetFlag = spActor->GetActorBitsetFlag();
		if (actorBitsetFlag.IsBitOn(EActorFlag::RenderingTarget) == false)
		{
			continue;
		}

		m_vecRenderingTargetActor.push_back(spActor);
	}

	// Actor를 Y값으로 정렬
	std::sort(m_vecRenderingTargetActor.begin(), m_vecRenderingTargetActor.end(),
		[] (const ActorPtr& spLhs, const ActorPtr& spRhs)
		{
			const TransformComponent* pLhsTransformComponent = spLhs->BringTransformComponent();
			const TransformComponent* pRhsTransformComponent = spRhs->BringTransformComponent();

			return (pLhsTransformComponent->GetPositionY() < pRhsTransformComponent->GetPositionY());
		});
}