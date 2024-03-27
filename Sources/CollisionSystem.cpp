#include "Pch.h"
#include "CollisionSystem.h"

#include "SystemManager.h"
#include "SceneSystem.h"
#include "SceneBase.h"
#include "Actor.h"
#include "ColliderComponent.h"

CollisionSystem::CollisionSystem()
{

}

CollisionSystem::~CollisionSystem()
{

}

bool CollisionSystem::Update()
{
	SceneBase::Ptr spScene = GET_SYSTEM(SceneSystem)->GetCurrentScene();
	std::vector<Actor*>& refActors = spScene->GetActors();

	// 씬에 있는 액터의 모든 콜라이더 컴포넌트 가져오기
	std::vector<ColliderComponent::Ptr> colliderComponents;
	for (Actor* pActor : refActors)
	{
		ColliderComponent::Ptr spColliderComponent = GET_COMPONENT(pActor, ColliderComponent);
		if (spColliderComponent != nullptr)
		{
			colliderComponents.emplace_back(spColliderComponent);
		}
	}

	// 2중으로 컬리전 체크
	uint32 colliderCount = colliderComponents.size();
	for (uint32 i = 0; i < colliderCount; ++i)
	{
		// 콜라이더의 액터 조사
		ColliderComponent::Ptr spFirstColliderComponent = colliderComponents[i];
		Actor* pFirstActor = spFirstColliderComponent->GetOwner();
		if (pFirstActor->GetActorState() == EActorState::Deactivated)
		{
			continue;
		}

		for (uint32 j = i + 1; j < colliderCount; ++j)
		{
			ColliderComponent::Ptr spSecondCollideComponent = colliderComponents[j];
			Actor* pSecondActor = spSecondCollideComponent->GetOwner();

			if (spFirstColliderComponent->TestIntersect(spSecondCollideComponent) == true) // 충돌
			{
				pFirstActor->SetActorState(EActorState::Deactivated);
				pSecondActor->SetActorState(EActorState::Deactivated);

				// 이걸 이용하면 콜백 처리도 가능

				GET_SYSTEM(SceneSystem)->AddRemoveDeactivatedActor(refActors, pFirstActor);
				GET_SYSTEM(SceneSystem)->AddRemoveDeactivatedActor(refActors, pSecondActor);
			}
		}
	}

	GET_SYSTEM(SceneSystem)->RemoveDeactivatedAtors();
	return false;
}

void CollisionSystem::Render()
{
	
}