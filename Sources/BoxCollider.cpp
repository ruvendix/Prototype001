#include "Pch.h"
#include "BoxCollider.h"

#include "GameApplication.h"

BoxCollider::BoxCollider()
{

}

BoxCollider::~BoxCollider()
{

}

bool BoxCollider::Update()
{
	Point2d cameraOffsetPos;
	if (m_bUseCamera == true)
	{
		Camera::Ptr spCamera = GameApplication::I()->GetCurrentCamera();
		cameraOffsetPos = spCamera->GetOffsetPosition();
	}

	m_boxRect.left   = (m_pos.x - static_cast<int32>(m_extents.width)) - cameraOffsetPos.x;
	m_boxRect.top    = (m_pos.y - static_cast<int32>(m_extents.height)) - cameraOffsetPos.y;
	m_boxRect.right  = (m_pos.x + static_cast<int32>(m_extents.width)) - cameraOffsetPos.x;
	m_boxRect.bottom = (m_pos.y + static_cast<int32>(m_extents.height)) - cameraOffsetPos.y;

	return true;
}

void BoxCollider::Render()
{
#ifdef _DEBUG
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
	if (spTargetBoxCollider != nullptr) // 박스 VS 박스
	{
		const RECT& targetBoxRect = spTargetBoxCollider->GetBoxRect();
		if ((m_boxRect.left < targetBoxRect.right) &&
			(m_boxRect.right > targetBoxRect.left) &&
			(m_boxRect.top < targetBoxRect.bottom) &&
			(m_boxRect.bottom > targetBoxRect.top))
		{
			return true;
		}
	}

	return false;
}