// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using TimerCallback = std::function<void()>;

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void Update(float deltaSeconds);
    void Reset();

    void SetTargetTime(float targetTime) { m_targetTime = targetTime; }
    void SetLoop(bool bLoop) { m_bLoop = bLoop; }
    void SetTimerCallback(const TimerCallback& timerCallback) { m_timerCallback = timerCallback; }
    void SetSwitch(bool bOn) { m_bOn = bOn; }

    float GetLocalTime() const { return m_localTime; }

private:
    float m_targetTime = 0.0f;
    float m_localTime = 0.0f;

    bool m_bOn = true;
    bool m_bLoop = false;

    TimerCallback m_timerCallback = nullptr;
};