#include "Pch.h"
#include "BoxCollider.h"

#include "GameApplication.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "SystemManager.h"
#include "CollisionSystem.h"

BoxCollider::BoxCollider()
{
	::ZeroMemory(&m_boxRect, sizeof(RECT));
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::Startup()
{
	UpdateBoxRect();
}

bool BoxCollider::Update()
{
	UpdateBoxRect();
	return true;
}

void BoxCollider::Render()
{
#ifdef _DEBUG
	Point2d cameraOffsetPos;
	if (m_bUseCamera == true)
	{
		Camera::Ptr spCamera = GameApplication::I()->GetCurrentCamera();
		if (spCamera != nullptr)
		{
			cameraOffsetPos = spCamera->GetOffsetPosition();
		}
	}

	// 렌더링 되기 직전에 카메라 오프셋 적용
	m_boxRect.left   -= cameraOffsetPos.x;
	m_boxRect.top    -= cameraOffsetPos.y;
	m_boxRect.right  -= cameraOffsetPos.x;
	m_boxRect.bottom -= cameraOffsetPos.y;

	HDC hBackbufferDc = GameApplication::I()->GetBackBufferDc();
	//::MoveToEx(hBackbufferDc, debugRect.left, debugRect.top, nullptr);
	//::LineTo(hBackbufferDc, debugRect.right, debugRect.top);
	//::LineTo(hBackbufferDc, debugRect.right, debugRect.bottom);
	//::LineTo(hBackbufferDc, debugRect.left, debugRect.bottom);
	//::LineTo(hBackbufferDc, debugRect.left, debugRect.top);

	HBRUSH hBursh = static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
	HBRUSH hPrevBrush = static_cast<HBRUSH>(::SelectObject(hBackbufferDc, hBursh));
	::Rectangle(hBackbufferDc, m_boxRect.left, m_boxRect.top, m_boxRect.right, m_boxRect.bottom);
	::SelectObject(hBackbufferDc, hPrevBrush);
#endif
}

bool BoxCollider::TestIntersect(ColliderBase::Ptr spTargetCollider)
{
	// 박스인지?
	BoxCollider::Ptr spTargetBoxCollider = std::dynamic_pointer_cast<BoxCollider>(spTargetCollider);
	if ((spTargetBoxCollider != nullptr) && // 박스 VS 박스
		(TestIntersectBox(spTargetBoxCollider) == true))
	{
		return true;
	}

	return false;
}

void BoxCollider::UpdateBoxRect()
{
	// 액터의 트랜스폼 가져오기
	TransformComponent::Ptr spTransformComponent = GET_COMPONENT(GetOwner(), TransformComponent);
	const Point2d& pos = spTransformComponent->GetPosition();

	m_boxRect.left   = (pos.x - static_cast<int32>(m_extents.width));
	m_boxRect.top    = (pos.y - static_cast<int32>(m_extents.height));
	m_boxRect.right  = (pos.x + static_cast<int32>(m_extents.width));
	m_boxRect.bottom = (pos.y + static_cast<int32>(m_extents.height));
}

bool BoxCollider::TestIntersectBox(BoxCollider::Ptr spTargetBoxCollider)
{
	const RECT& targetBoxRect = spTargetBoxCollider->GetBoxRect();
	//if ((m_boxRect.left > targetBoxRect.right) ||
	//	(m_boxRect.right < targetBoxRect.left) ||
	//	(m_boxRect.top > targetBoxRect.bottom) ||
	//	(m_boxRect.bottom < targetBoxRect.top))
	//{
	//	return false;
	//}

	RECT intersectedRect;

	//// 경우에 따라서 겹쳐진 정도 구하기
	//bool bRightToLeft = false;
	//bool bLeftToRight = false;

	//if ((targetBoxRect.left < m_boxRect.left) &&
	//	(m_boxRect.left < targetBoxRect.right))
	//{
	//	bRightToLeft = true;

	//	intersectedRect.left = m_boxRect.left;
	//	intersectedRect.right = targetBoxRect.right;
	//}

	//if ((targetBoxRect.left < m_boxRect.right) &&
	//	(m_boxRect.right < targetBoxRect.right))
	//{
	//	bLeftToRight = true;

	//	intersectedRect.left = targetBoxRect.left;
	//	intersectedRect.right = m_boxRect.right;
	//}

	//if ((bRightToLeft == true) &&
	//	(bLeftToRight == true))
	//{
	//	intersectedRect.left = m_boxRect.left;
	//	intersectedRect.right = m_boxRect.right;
	//}
	////////////////////////////////////////////////////
	//bool bTopToBottom = false;
	//bool bBottomToTop = false;

	//if ((targetBoxRect.top < m_boxRect.top) &&
	//	(m_boxRect.top < targetBoxRect.bottom))
	//{
	//	bTopToBottom = true;

	//	intersectedRect.top = m_boxRect.top;
	//	intersectedRect.bottom = targetBoxRect.bottom;
	//}

	//if ((targetBoxRect.top < m_boxRect.bottom) &&
	//	(m_boxRect.bottom < targetBoxRect.bottom))
	//{
	//	bBottomToTop = true;

	//	intersectedRect.top = targetBoxRect.top;
	//	intersectedRect.bottom = m_boxRect.bottom;
	//}

	//if ((bTopToBottom == true) &&
	//	(bBottomToTop == true))
	//{
	//	intersectedRect.top = m_boxRect.top;
	//	intersectedRect.bottom = m_boxRect.bottom;
	//}

	if (::IntersectRect(&intersectedRect, &m_boxRect, &targetBoxRect) == FALSE)
	{
		return false;
	}

	m_intersectedRectIdx = GET_SYSTEM(CollisionSystem)->AddIntersectedRect(intersectedRect);
	return true;
}