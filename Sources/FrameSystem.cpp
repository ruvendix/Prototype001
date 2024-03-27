/*
<내용 요약>
프레임을 관리하는 시스템
*/

#include "Pch.h"
#include "FrameSystem.h"

#include "GameApplication.h"

namespace
{
	float g_fpsLocalTime = 0.0f;
}

FrameSystem::FrameSystem()
{

}

FrameSystem::~FrameSystem()
{

}

void FrameSystem::Startup()
{
	::QueryPerformanceFrequency(&m_frequency);
	::QueryPerformanceCounter(&m_prevCounter);
}

bool FrameSystem::Update()
{
	LARGE_INTEGER currentCounter;
	::QueryPerformanceCounter(&currentCounter);
	m_deltaSeconds = ((currentCounter.QuadPart - m_prevCounter.QuadPart) / static_cast<float>(m_frequency.QuadPart)); // 초 단위

	// 고정 60 프레임
	float remainDeltaSeconds = 0.016f - m_deltaSeconds;
	if (remainDeltaSeconds > 0.0f)
	{
		::Sleep(static_cast<DWORD>(remainDeltaSeconds * 1000.0f));
	}

	m_prevCounter = currentCounter;

	++m_frameCount;

	g_fpsLocalTime += m_deltaSeconds;
	if (g_fpsLocalTime >= 1.0f)
	{
		m_fps = m_frameCount - m_prevFpsFrameCount;
		m_prevFpsFrameCount = m_frameCount;

		g_fpsLocalTime = 0.0f;
	}

	m_strFrameGuide = std::format("Frame: {0}, Fps: {1}", m_frameCount, m_fps);
	return true;
}