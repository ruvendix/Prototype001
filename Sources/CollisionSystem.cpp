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

	// ���� �ִ� ������ ��� �ݶ��̴� ������Ʈ ��������
	std::vector<CollisionComponentPtr> collisoinComponents;
	for (auto actors : refActors)
	{
		for (Actor* pActor : actors)
		{
			FIND_COMPONENTS(pActor, CollisionComponent, collisoinComponents);
		}
	}

	// 2������ �ø��� üũ
	uint32 collisionComponentCount = collisoinComponents.size();
	for (uint32 i = 0; i < collisionComponentCount; ++i)
	{
		// �ݶ��̴��� ���� ����
		CollisionComponentPtr spLhsCollisionComponent = collisoinComponents[i];

		// ��Ȱ���� ���Ͷ�� ����
		Actor* pFirstActor = spLhsCollisionComponent->GetOwner();
		if (pFirstActor->GetActorState() == EActorState::Deactivated)
		{
			continue;
		}

		for (uint32 j = i + 1; j < collisionComponentCount; ++j)
		{
			CollisionComponentPtr spRhsCollisionComponent = collisoinComponents[j];
			//Actor* pSecondActor = spSecondCollideComponent->GetOwner();

			// Lhs�� Rhs�� �ø��� �����ϵ��� �Ǿ��ִ���?
			ECollisionResponseState toRhsCollisionResponseState = spLhsCollisionComponent->TestCollisionResponseBit(spRhsCollisionComponent->GetCollisionObjectType());
			ECollisionResponseState toLhsCollisionResponseState = spRhsCollisionComponent->TestCollisionResponseBit(spLhsCollisionComponent->GetCollisionObjectType());

			// �ֹ� ���ø� �н�
			if ((toRhsCollisionResponseState == ECollisionResponseState::Ignore) &&
				(toLhsCollisionResponseState == ECollisionResponseState::Ignore))
			{
				continue;
			}

			// �浹 �� ��ħ �˻�� ���ʸ� �����ص� ��
			if (spLhsCollisionComponent->TestIntersect(spRhsCollisionComponent) == false)
			{
				continue;
			}

			// ������� ����Ǹ� �浹 �� ��ħ Ȯ��
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