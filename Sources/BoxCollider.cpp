#include "Pch.h"
#include "BoxCollider.h"

#include "GameApplication.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "SystemManager.h"
#include "CollisionSystem.h"
#include "DebugSystem.h"
#include "Camera.h"

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

bool BoxCollider::PostUpdate()
{
	UpdateBoxRect();
	return true;
}

void BoxCollider::Render()
{
	if (GET_SYSTEM(DebugSystem)->IsEnableOption(EDebugOption::ShowCollider) == false)
	{
		return;
	}

	Point2d cameraOffsetPos;
	if (m_bUseCamera == true)
	{
		CameraPtr spCamera = GameApplication::I()->GetCurrentCamera();
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

	HPEN hPen = ::CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
	HPEN hPrevPen = static_cast<HPEN>(::SelectObject(hBackbufferDc, hPen));

	::Rectangle(hBackbufferDc, m_boxRect.left, m_boxRect.top, m_boxRect.right, m_boxRect.bottom);
	::SelectObject(hBackbufferDc, hPrevBrush);
	::SelectObject(hBackbufferDc, hPrevPen);

	::DeleteObject(hPen);
}

bool BoxCollider::TestIntersect(ColliderBasePtr spTargetCollider)
{
	// 박스인지?
	BoxColliderPtr spTargetBoxCollider = std::dynamic_pointer_cast<BoxCollider>(spTargetCollider);
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
	TransformComponentPtr spTransformComponent = GET_COMPONENT(GetActor(), TransformComponent);
	const Point2d& pos = spTransformComponent->GetPosition();

	m_boxRect.left   = (pos.x - static_cast<int32>(m_extents.width));
	m_boxRect.top    = (pos.y - static_cast<int32>(m_extents.height));
	m_boxRect.right  = (pos.x + static_cast<int32>(m_extents.width));
	m_boxRect.bottom = (pos.y + static_cast<int32>(m_extents.height));
}

bool BoxCollider::TestIntersectBox(BoxColliderPtr spTargetBoxCollider)
{
	RECT intersectedRect;
	const RECT& targetBoxRect = spTargetBoxCollider->GetBoxRect();

	if (::IntersectRect(&intersectedRect, &m_boxRect, &targetBoxRect) == FALSE)
	{
		return false;
	}

	m_intersectedRectIdx = GET_SYSTEM(CollisionSystem)->AddIntersectedRect(intersectedRect);
	spTargetBoxCollider->SetInstersectedRectIndex(m_intersectedRectIdx);

	return true;
}