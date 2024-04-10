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

void CollisionSystem::Startup()
{
	// �̸� ���� �ø��� ���� ����
	m_globalCollisionResponseInfos.emplace_back("WorldStatic", ECollisionResponseState::Ignore);
	m_globalCollisionResponseInfos.emplace_back("Pawn", ECollisionResponseState::Ignore); // �̰� ĳ����
}

bool CollisionSystem::Update()
{
	SceneBase::Ptr spScene = GET_SYSTEM(SceneSystem)->GetCurrentScene();
	SceneBase::Actors& refActors = spScene->GetActors();

	// ���� �ִ� ������ ��� �ݶ��̴� ������Ʈ ��������
	std::vector<CollisionComponent::Ptr> collisoinComponents;
	for (auto actors : refActors)
	{
		for (Actor* pActor : actors)
		{
			CollisionComponent::Ptr spCollisionComponent = GET_COMPONENT(pActor, CollisionComponent);
			if (spCollisionComponent != nullptr)
			{
				collisoinComponents.emplace_back(spCollisionComponent);
			}
		}
	}

	// 2������ �ø��� üũ
	uint32 collisionComponentCount = collisoinComponents.size();
	for (uint32 i = 0; i < collisionComponentCount; ++i)
	{
		// �ݶ��̴��� ���� ����
		CollisionComponent::Ptr spLhsCollisionComponent = collisoinComponents[i];

		// ��Ȱ���� ���Ͷ�� ����
		Actor* pFirstActor = spLhsCollisionComponent->GetOwner();
		if (pFirstActor->GetActorState() == EActorState::Deactivated)
		{
			continue;
		}

		for (uint32 j = i + 1; j < collisionComponentCount; ++j)
		{
			CollisionComponent::Ptr spRhsCollisionComponent = collisoinComponents[j];
			//Actor* pSecondActor = spSecondCollideComponent->GetOwner();

			// Lhs�� Rhs�� �ø��� �����ϵ��� �Ǿ��ִ���?
			const CollisionResponseInfo& toRhsCollisionResponseInfo = spLhsCollisionComponent->FindCollisionResponseInfo(spRhsCollisionComponent->GetObjectTypeTag());
			const CollisionResponseInfo& toLhsCollisionResponseInfo = spRhsCollisionComponent->FindCollisionResponseInfo(spLhsCollisionComponent->GetObjectTypeTag());

			// Lhs -> Rhs
			bool bLhsToRhsCollisionResponse = false;
			if ((toRhsCollisionResponseInfo.strObjectTypeTag != CollisionComponent::s_szNullObjectType) &&
				(toRhsCollisionResponseInfo.collisionResponseState != ECollisionResponseState::Ignore))
			{
				// ������� ������ Lhs�� Rhs�� ���� Block�̰ų� Overlap
				bLhsToRhsCollisionResponse = true;
			}

			// Rhs -> Lhs
			bool bRhsToLhsCollisionResponse = false;
			if ((toLhsCollisionResponseInfo.strObjectTypeTag != CollisionComponent::s_szNullObjectType) &&
				(toLhsCollisionResponseInfo.collisionResponseState != ECollisionResponseState::Ignore))
			{
				// ������� ������ Rhs�� Lhs�� ���� Block�̰ų� Overlap
				bRhsToLhsCollisionResponse = true;
			}

			// ���� ���� �ø��� ������ ������ ����
			if ((bLhsToRhsCollisionResponse == false) &&
				(bRhsToLhsCollisionResponse == false))
			{
				continue;
			}

			// �浹 �� ��ħ �˻�� ���ʸ� �����ص� ��
			if (spLhsCollisionComponent->TestIntersect(spRhsCollisionComponent) == false)
			{
				continue;
			}

			// ������� ����Ǹ� �浹 �� ��ħ Ȯ��

			if (toRhsCollisionResponseInfo.collisionResponseState == ECollisionResponseState::Block)
			{
				spLhsCollisionComponent->ProcessHitEvent(spRhsCollisionComponent);
			}
			else if (toRhsCollisionResponseInfo.collisionResponseState == ECollisionResponseState::Overlap)
			{
				spLhsCollisionComponent->ProcessOverlapEvent(spRhsCollisionComponent);
			}

			if (toLhsCollisionResponseInfo.collisionResponseState == ECollisionResponseState::Block)
			{
				spRhsCollisionComponent->ProcessHitEvent(spLhsCollisionComponent);
			}
			else if (toLhsCollisionResponseInfo.collisionResponseState == ECollisionResponseState::Overlap)
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
	Camera::Ptr spCamera = GameApplication::I()->GetCurrentCamera();
	cameraOffsetPos = spCamera->GetOffsetPosition();

	for (RECT& intersectedRect : m_intersectedRects)
	{
		//::Rectangle(hBackbufferDc, intersectedRect.left, intersectedRect.top, intersectedRect.right, intersectedRect.bottom);
	}
	
	m_intersectedRects.clear();

	::SelectObject(hBackbufferDc, hPrevBrush);

	::SelectObject(hBackbufferDc, hPrevPen);
	::DeleteObject(hPen);
#endif
}

CollisionResponseInfo CollisionSystem::CreateCollisionResponseInfo(const std::string& strObjectTypeTag)
{
	const auto& foundIter = std::find_if(m_globalCollisionResponseInfos.begin(), m_globalCollisionResponseInfos.end(),
		[strObjectTypeTag] (CollisionResponseInfo& elem)
		{
			return (strObjectTypeTag == elem.strObjectTypeTag);
		});

	if (foundIter == m_globalCollisionResponseInfos.cend())
	{
		return CollisionComponent::s_nullCollisionResponseInfo;
	}

	return (*foundIter);
}

int32 CollisionSystem::AddIntersectedRect(const RECT& intersectedRect)
{
	m_intersectedRects.emplace_back(intersectedRect);
	return static_cast<int32>(m_intersectedRects.size() - 1);
}