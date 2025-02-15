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
    if (m_localTime > m_triggerTime)
    {
        // 반복인가?
        if (m_bLoop == false)
        {
            m_bOn = false;
        }

        // 델리게이터가 있는지?
        if (m_timerDelegator.IsBoundAnyFixedArgumentsFunction())
        {
            m_timerDelegator.InvokeFixedArgumentFunctions();
        }

        // 로컬 타임은 콜백 처리가 끝난 후에 갱신
        m_localTime -= m_triggerTime;
    }
}

void Timer::Reset()
{
    m_localTime = 0.0f;
}