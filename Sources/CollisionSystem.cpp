#include "Pch.h"
#include "CollisionSystem.h"

#include "SystemManager.h"
#include "SceneSystem.h"
#include "SceneBase.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "GameApplication.h"
#include "Camera.h"

CollisionSystem::CollisionSystem()
{

}

CollisionSystem::~CollisionSystem()
{

}

bool CollisionSystem::PostUpdate()
{
	SceneBasePtr spScene = GET_SYSTEM(SceneSystem)->GetCurrentScene();
	SceneBase::Actors& refActors = spScene->GetActors();

	// 씬에 있는 액터의 모든 콜라이더 컴포넌트 가져오기
	std::vector<CollisionComponentPtr> collisoinComponents;
	for (auto actors : refActors)
	{
		for (Actor* pActor : actors)
		{
			FIND_COMPONENTS(pActor, CollisionComponent, collisoinComponents);
		}
	}

	// 2중으로 컬리전 체크
	uint32 collisionComponentCount = collisoinComponents.size();
	for (uint32 i = 0; i < collisionComponentCount; ++i)
	{
		// 콜라이더의 액터 조사
		CollisionComponentPtr spLhsCollisionComponent = collisoinComponents[i];

		// 비활성된 액터라면 생략
		Actor* pFirstActor = spLhsCollisionComponent->GetOwner();
		if (pFirstActor->GetActorState() == EActorState::Deactivated)
		{
			continue;
		}

		for (uint32 j = i + 1; j < collisionComponentCount; ++j)
		{
			CollisionComponentPtr spRhsCollisionComponent = collisoinComponents[j];
			//Actor* pSecondActor = spSecondCollideComponent->GetOwner();

			// Lhs가 Rhs와 컬리전 반응하도록 되어있는지?
			ECollisionResponseState toRhsCollisionResponseState = spLhsCollisionComponent->TestCollisionResponseBit(spRhsCollisionComponent->GetCollisionObjectType());
			ECollisionResponseState toLhsCollisionResponseState = spRhsCollisionComponent->TestCollisionResponseBit(spLhsCollisionComponent->GetCollisionObjectType());

			// 쌍방 무시면 패스
			if ((toRhsCollisionResponseState == ECollisionResponseState::Ignore) &&
				(toLhsCollisionResponseState == ECollisionResponseState::Ignore))
			{
				continue;
			}

			// 충돌 및 겹침 검사는 한쪽만 진행해도 됨
			if (spLhsCollisionComponent->TestIntersect(spRhsCollisionComponent) == false)
			{
				continue;
			}

			// 여기까지 진행되면 충돌 및 겹침 확정
			if (toRhsCollisionResponseState == ECollisionResponseState::Block)
			{
				spLhsCollisionComponent->ProcessHitEvent(spRhsCollisionComponent);
			}
			else if (toRhsCollisionResponseState == ECollisionResponseState::Overlap)
			{
				spLhsCollisionComponent->ProcessOverlapEvent(spRhsCollisionComponent);
			}

			if (toLhsCollisionResponseState == ECollisionResponseState::Block)
			{
				spRhsCollisionComponent->ProcessHitEvent(spLhsCollisionComponent);
			}
			else if (toLhsCollisionResponseState == ECollisionResponseState::Overlap)
			{
				spRhsCollisionComponent->ProcessOverlapEvent(spLhsCollisionComponent);
			}
		}
	}

	GET_SYSTEM(SceneSystem)->RemoveDeactivatedAtors();
	return false;
}

void CollisionSystem::Render()
{
#ifdef _DEBUG
	HDC hBackbufferDc = GameApplication::I()->GetBackBufferDc();
	HBRUSH hBursh = static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
	HBRUSH hPrevBrush = static_cast<HBRUSH>(::SelectObject(hBackbufferDc, hBursh));

	HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hPrevPen = static_cast<HPEN>(::SelectObject(hBackbufferDc, hPen));

	Point2d cameraOffsetPos;
	CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();
	cameraOffsetPos = spCamera->GetOffsetPosition();

	for (RECT& intersectedRect : m_intersectedRects)
	{
		::Rectangle(hBackbufferDc,
			intersectedRect.left - cameraOffsetPos.x,
			intersectedRect.top - cameraOffsetPos.y,
			intersectedRect.right - cameraOffsetPos.x,
			intersectedRect.bottom - cameraOffsetPos.y);
	}
	
	m_intersectedRects.clear();

	::SelectObject(hBackbufferDc, hPrevBrush);

	::SelectObject(hBackbufferDc, hPrevPen);
	::DeleteObject(hPen);
#endif
}

int32 CollisionSystem::AddIntersectedRect(const RECT& intersectedRect)
{
	m_intersectedRects.emplace_back(intersectedRect);
	return static_cast<int32>(m_intersectedRects.size() - 1);
}