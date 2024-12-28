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
        // �ݺ��ΰ�?
        if (m_bLoop == false)
        {
            m_bOn = false;
        }

        // �ݹ��� �ִ°�?
        if (m_timerCallback != nullptr)
        {
            m_timerCallback(m_callbackArgs);
        }

        // ���� Ÿ���� �ݹ� ó���� ���� �Ŀ� ����
        m_localTime -= m_targetTime;
    }
}

void Timer::Reset()
{
    m_localTime = 0.0f;
}

void Timer::AddTimerCallbackArg(const std::any& timerCallbackArg)
{
    m_callbackArgs.push_back(timerCallbackArg);
}