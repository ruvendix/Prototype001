// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "MouseDevice.h"

bool MouseDevice::Update(float deltaSeconds)
{
	// 마우스 좌표는 최초 1번만 갱신
	POINT currentMousePos;
	::GetCursorPos(&currentMousePos);
	::ScreenToClient(SceneRenderer::I()->GetViewerHandle(), &currentMousePos);

	// 변환한 마우스 좌표 저장
	m_currentMousePos.x = currentMousePos.x;
	m_currentMousePos.y = currentMousePos.y;

	//DEFAULT_TRACE_LOG("마우스 좌표(%d, %d)", m_currentMousePos.x, m_currentMousePos.y);
	return true;
}