// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Timer.h"

void Timer::Update(float deltaSeconds)
{
    if (m_bOn == false)
    {
        return;
    }

    m_localTime += deltaSeconds;
    if (m_localTime > m_targetTime)
    {
        // 반복인가?
        if (m_bLoop == false)
        {
            m_bOn = false;
        }

        // 콜백이 있는가?
        if (m_timerCallback != nullptr)
        {
            m_timerCallback();
        }

        // 로컬 타임은 콜백 처리가 끝난 후에 갱신
        m_localTime -= m_targetTime;
    }
}

void Timer::Reset()
{
    m_localTime = 0.0f;
}