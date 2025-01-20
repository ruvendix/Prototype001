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

	// ���� ������Ʈ�� 2���� �Ǵ� ���� ����
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

	// ���࿡ Ÿ�� ���Ͱ� ���ʶ�� ����
	if (spMainCameraTargetActor.get() == pCameraComponent->GetOwner())
	{
		TRACE_LOG(LogDefault, "Ÿ�� ���Ͱ� ���� ī�޶��� ���ʶ� ���� �Ұ���!");
		return;
	}

	pCameraComponent->SetTargetActor(spMainCameraTargetActor);
	SceneRenderer::I()->SetMainCameraActor(m_spMainCameraActor);
}