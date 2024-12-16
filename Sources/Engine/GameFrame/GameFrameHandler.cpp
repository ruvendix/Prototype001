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
	// ��������� �������� ���õ� ��, ������Ʈ�� �ƹ� ���� ����
	//bool isVSyncOn = m_pContext->GetVideoInfo()->m_bVSync;

	// ���� ������ => ���� �����Ӱ� ���� ������ ������ �׻� �����ϰ� ����(�߰� ó�� ����), ������ ��� �� ó���� ����, ������ ���� ���ϴ� �� ����
	// ���� ������ => ���� �����Ӱ� ���� ������ ������ �����̰� �װ� �ݿ�(�߰� ó�� ����), ������ ��� �� ������ ���ݸ�ŭ ���, ������ �������� ����

	// ���������� �������� ���� üũ
	LARGE_INTEGER currentFrameTime = MeasureTime();

	// ��ŸŸ�� ��� (m_i64PrevTimeStampEachFrame ���� ������ ���� ����)	
	m_deltaSeconds = CalculateTimeInterval(m_prevFrameTime, currentFrameTime);

	// ������ �������̸� ������ Ÿ�Ӹ� ����
	if (m_frameRate == -1)
	{
		m_prevFrameTime = currentFrameTime;
		return;
	}

	float mustWaitTime = (m_targetFrameRateSeconds - m_deltaSeconds);
	if (mustWaitTime > 0.0f) // �̹� �������� ��ǥ �ð����� ���� ó���Ǿ����� ���
	{
		if (mustWaitTime > 0.01f) // 10ms���� ������� while ���Ǵ����� ó��
		{
			DWORD dwSleepPeriod = static_cast<DWORD>(mustWaitTime * 1000.0f);
			::timeBeginPeriod(1);
			::Sleep(dwSleepPeriod); // ������ ���� ���ؽ�Ʈ ����Ī �ڽ�Ʈ ������ �ð����� �۴ٸ� while ���Ǵ����� ó��
			::timeEndPeriod(1);

			// �����Ѹ�ŭ �������� ��ŸŸ�� ����
			m_deltaSeconds = m_targetFrameRateSeconds;
		}
		else
		{
			// while ���Ǵ�
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

	// ��� ó���� �������� �ٽ� �ð� ����
	m_prevFrameTime = MeasureTime();

	// ������ ī��Ʈ
	++m_frameCount;
}

void GameFrameHandler::ApplyFrameRate(int32 frameRate)
{
	m_frameRate = frameRate;
	m_targetFrameRateSeconds = (1.0f / m_frameRate);
}

void GameFrameHandler::OnTimerFps()
{
	// 1�� ���� ������ ī��Ʈ�� �˰� �ִٸ�?
	int32 fps = ((m_frameCount - m_frameCountBeforeSecond) / static_cast<int32>(m_frameTimer.GetLocalTime()));
	m_frameCountBeforeSecond = m_frameCount;

	std::string strFps = "Fps: ";
	strFps += (std::to_string(fps) + '\n');
	::OutputDebugString(strFps.c_str());
}