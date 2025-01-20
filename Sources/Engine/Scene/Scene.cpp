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
	for (int32 i = 0; i < TO_NUM(EUpdateOrder::Count); ++i)
	{
		std::vector<ActorPtr>& refVecUpdateActor = m_arrUpdateActors[i];
		for (ActorPtr& spActor : refVecUpdateActor)
		{
			spActor->Update(deltaSeconds);
		}
	}

	// 씬은 업데이트가 2종류 또는 여러 종류
	return true;
}

void Scene::Cleanup()
{
	for (ActorPtr& spActor : m_vecActors)
	{
		spActor->Cleanup();
	}

	m_vecActors.clear();
}

void Scene::RegisterMainCameraActorToScene(const ActorPtr& spMainCameraTargetActor)
{
	m_spMainCameraActor = CreateActorToScene<CameraActor>(EUpdateOrder::Post);
	
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