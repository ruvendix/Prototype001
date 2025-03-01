// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "MouseDevice.h"

bool MouseDevice::Update(float deltaSeconds)
{
	// ���콺 ��ǥ�� ���� 1���� ����
	POINT currentMousePos;
	::GetCursorPos(&currentMousePos);
	::ScreenToClient(SceneRenderer::I()->GetViewerHandle(), &currentMousePos);

	// ��ȯ�� ���콺 ��ǥ ����
	m_currentMousePos.x = currentMousePos.x;
	m_currentMousePos.y = currentMousePos.y;

	//DEFAULT_TRACE_LOG("���콺 ��ǥ(%d, %d)", m_currentMousePos.x, m_currentMousePos.y);
	return true;
}