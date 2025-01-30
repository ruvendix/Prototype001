// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void Update(float deltaSeconds);
    void Reset();

    void SetTriggerTime(float triggerTime) { m_triggerTime = triggerTime; }
    void SetLoop(bool bLoop) { m_bLoop = bLoop; }
    void SetSwitch(bool bOn) { m_bOn = bOn; }
    void SetTimerCallback(const TimerCallback& timerCallback) { m_timerCallback = timerCallback; }

    float GetLocalTime() const { return m_localTime; }

private:
    float m_triggerTime = 0.0f;
    float m_localTime = 0.0f;

    bool m_bOn = true;
    bool m_bLoop = false;

    TimerCallback m_timerCallback = nullptr;
};