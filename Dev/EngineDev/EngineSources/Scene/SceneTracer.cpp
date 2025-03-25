// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SceneTracer.h"

#include "EngineSources/Scene/SceneManager.h"

DEFINE_SINGLETON(SceneTracer);

void SceneTracer::Tracer()
{
	LayerActors renderableLayerActors;

	Scene* pScene = SceneManager::I()->GetCurrentScene();
	ASSERT_LOG_RETURN(pScene != nullptr);
	for (const ActorPtr& spActor : pScene->GetActors())
	{
		TransformComponent* pTransformComponent = spActor->GetComponent<TransformComponent>();
		ASSERT_LOG(pTransformComponent != nullptr);
		pTransformComponent->BuildWolrdMatrix();

		const Actor::ActorBitset& actorBitsetFlag = spActor->GetActorBitsetFlag();
		if (actorBitsetFlag.IsBitOn(EActorFlag::RenderingTarget) == false)
		{
			continue;
		}

		renderableLayerActors[TO_NUM(spActor->GetActorLayer())].push_back(spActor);
	}

	// Actor�� Y������ ���� (�ϴ��� Creature��)
	std::sort(renderableLayerActors[TO_NUM(EActorLayerType::Creature)].begin(),
		renderableLayerActors[TO_NUM(EActorLayerType::Creature)].end(),
		[](const ActorPtr& spLhs, const ActorPtr& spRhs)
		{
			const TransformComponent* pLhsTransformComponent = spLhs->GetComponent<TransformComponent>();
			ASSERT_LOG(pLhsTransformComponent != nullptr);

			const TransformComponent* pRhsTransformComponent = spRhs->GetComponent<TransformComponent>();
			ASSERT_LOG(pRhsTransformComponent != nullptr);

			return (pLhsTransformComponent->GetPositionY() < pRhsTransformComponent->GetPositionY());
		});

#pragma region �������� ���� ������Ʈ ����
	m_vecOutputRenderComponent.clear();
	for (const Actors& actors : renderableLayerActors)
	{
		for (const ActorPtr& spActor : actors)
		{
			// ���͸��� ���� ������Ʈ ��������
			spActor->FindRenderComponents(m_vecOutputRenderComponent);
		}
	}
#pragma endregion
}