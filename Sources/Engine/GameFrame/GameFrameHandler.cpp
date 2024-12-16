// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "GameFrameHandler.h"

DEFINE_SINGLETON(GameFrameHandler);

void GameFrameHandler::Startup()
{
	ApplyFrameRate(60);

	TimerCallback frameTimerCallback = std::bind(&GameFrameHandler::OnTimerFps, this);
	m_frameTimer = TimerManager::I()->CreateTimer(1.0f, true, frameTimerCallback);

	m_prevFrameTime = MeasureTime();
}

void GameFrameHandler::SyncFrameRate()
{
	// 수직동기는 렌더링과 관련된 것, 업데이트와 아무 관련 없음
	//bool isVSyncOn = m_pContext->GetVideoInfo()->m_bVSync;

	// 고정 프레임 => 이전 프레임과 현재 프레임 간격을 항상 일정하게 맞춤(추가 처리 있음), 프레임 드랍 시 처리는 무시, 프레임 숫자 정하는 건 이쪽
	// 가변 프레임 => 이전 프레임과 현재 프레임 간격이 랜덤이고 그걸 반영(추가 처리 없음), 프레임 드랍 시 벌어진 간격만큼 사용, 무제한 프레임은 이쪽

	// 렌더링까지 끝났으니 시작 체크
	LARGE_INTEGER currentFrameTime = MeasureTime();

	// 델타타임 계산 (m_i64PrevTimeStampEachFrame 이전 프레임 시작 지점)	
	m_deltaSeconds = CalculateTimeInterval(m_prevFrameTime, currentFrameTime);

	// 무제한 프레임이면 프레임 타임만 갱신
	if (m_frameRate == -1)
	{
		m_prevFrameTime = currentFrameTime;
		return;
	}

	float mustWaitTime = (m_targetFrameRateSeconds - m_deltaSeconds);
	if (mustWaitTime > 0.0f) // 이번 프레임은 목표 시간보다 빨리 처리되었으니 대기
	{
		if (mustWaitTime > 0.01f) // 10ms이하 정도라면 while 스피닝으로 처리
		{
			DWORD dwSleepPeriod = static_cast<DWORD>(mustWaitTime * 1000.0f);
			::timeBeginPeriod(1);
			::Sleep(dwSleepPeriod); // 슬립에 의한 컨텍스트 스위칭 코스트 때문에 시간차가 작다면 while 스피닝으로 처리
			::timeEndPeriod(1);

			// 슬립한만큼 지났으니 델타타임 고정
			m_deltaSeconds = m_targetFrameRateSeconds;
		}
		else
		{
			// while 스피닝
			while (m_deltaSeconds < m_targetFrameRateSeconds)
			{
				currentFrameTime = MeasureTime();
				m_deltaSeconds = CalculateTimeInterval(m_prevFrameTime, currentFrameTime);
			}
		}
	}
}

void GameFrameHandler::ProcessFramePerSeconds()
{
	m_frameTimer.Update(m_deltaSeconds);

	// 모든 처리가 끝났으니 다시 시간 측정
	m_prevFrameTime = MeasureTime();

	// 프레임 카운트
	++m_frameCount;
}

void GameFrameHandler::ApplyFrameRate(int32 frameRate)
{
	m_frameRate = frameRate;
	m_targetFrameRateSeconds = (1.0f / m_frameRate);
}

void GameFrameHandler::OnTimerFps()
{
	// 1초 전의 프레임 카운트를 알고 있다면?
	int32 fps = ((m_frameCount - m_frameCountBeforeSecond) / static_cast<int32>(m_frameTimer.GetLocalTime()));
	m_frameCountBeforeSecond = m_frameCount;

	std::string strFps = "Fps: ";
	strFps += (std::to_string(fps) + '\n');
	::OutputDebugString(strFps.c_str());
}