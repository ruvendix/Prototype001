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
	SceneBase::Actors& refActors = spScene->GetActors();

	// ���� �ִ� ������ ��� �ݶ��̴� ������Ʈ ��������
	std::vector<ColliderComponent::Ptr> colliderComponents;
	for (auto actors : refActors)
	{
		for (Actor* pActor : actors)
		{
			ColliderComponent::Ptr spColliderComponent = GET_COMPONENT(pActor, ColliderComponent);
			if (spColliderComponent != nullptr)
			{
				colliderComponents.emplace_back(spColliderComponent);
			}
		}
	}

	// 2������ �ø��� üũ
	uint32 colliderCount = colliderComponents.size();
	for (uint32 i = 0; i < colliderCount; ++i)
	{
		// �ݶ��̴��� ���� ����
		ColliderComponent::Ptr spFirstColliderComponent = colliderComponents[i];

		Actor* pFirstActor = spFirstColliderComponent->GetOwner();
		if (pFirstActor->GetActorState() == EActorState::Deactivated)
		{
			continue;
		}

		for (uint32 j = i + 1; j < colliderCount; ++j)
		{
			ColliderComponent::Ptr spSecondCollideComponent = colliderComponents[j];
			//Actor* pSecondActor = spSecondCollideComponent->GetOwner();

			// First�� Second�� ���� ������ �ִ���?
			ECollisionState collisionStateOfFirstCollider = spFirstColliderComponent->FindCollisonState(spSecondCollideComponent);

			if (spFirstColliderComponent->TestIntersect(spSecondCollideComponent) == true) // �浹
			{
				if (collisionStateOfFirstCollider == ECollisionState::None)
				{
					spFirstColliderComponent->SetCollisionState(spSecondCollideComponent, ECollisionState::Enter);
					spFirstColliderComponent->ProcessCollsionCallback(spSecondCollideComponent, ECollisionState::Enter);

					spSecondCollideComponent->SetCollisionState(spFirstColliderComponent, ECollisionState::Enter);
					spSecondCollideComponent->ProcessCollsionCallback(spFirstColliderComponent, ECollisionState::Enter);
				}
				else if ((collisionStateOfFirstCollider == ECollisionState::Enter) ||
						 (collisionStateOfFirstCollider == ECollisionState::Keep))
				{
					spFirstColliderComponent->SetCollisionState(spSecondCollideComponent, ECollisionState::Keep);
					spFirstColliderComponent->ProcessCollsionCallback(spSecondCollideComponent, ECollisionState::Keep);

					spSecondCollideComponent->SetCollisionState(spFirstColliderComponent, ECollisionState::Keep);
					spSecondCollideComponent->ProcessCollsionCallback(spFirstColliderComponent, ECollisionState::Keep);
				}

				//pFirstActor->SetActorState(EActorState::Deactivated);
				//pSecondActor->SetActorState(EActorState::Deactivated);

				//GET_SYSTEM(SceneSystem)->AddRemoveDeactivatedActor(refActors, pFirstActor);
				//GET_SYSTEM(SceneSystem)->AddRemoveDeactivatedActor(refActors, pSecondActor);
			}
			else
			{
				if ((collisionStateOfFirstCollider == ECollisionState::Enter) ||
					(collisionStateOfFirstCollider == ECollisionState::Keep))
				{
					spFirstColliderComponent->SetCollisionState(spSecondCollideComponent, ECollisionState::Leave);
					spFirstColliderComponent->ProcessCollsionCallback(spSecondCollideComponent, ECollisionState::Leave);

					spSecondCollideComponent->SetCollisionState(spFirstColliderComponent, ECollisionState::Leave);
					spSecondCollideComponent->ProcessCollsionCallback(spFirstColliderComponent, ECollisionState::Leave);
				}
				else if (collisionStateOfFirstCollider == ECollisionState::Leave)
				{
					spFirstColliderComponent->SetCollisionState(spSecondCollideComponent, ECollisionState::None);
					spFirstColliderComponent->EraseCollsionState(spSecondCollideComponent);

					spSecondCollideComponent->SetCollisionState(spFirstColliderComponent, ECollisionState::None);
					spSecondCollideComponent->EraseCollsionState(spFirstColliderComponent);
				}
			}
		}
	}

	GET_SYSTEM(SceneSystem)->RemoveDeactivatedAtors();
	return false;
}

void CollisionSystem::Render()
{
	
}