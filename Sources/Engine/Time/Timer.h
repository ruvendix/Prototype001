// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void Update(float deltaSeconds);
    void Reset();
    void AddTimerCallbackArg(const std::any& timerCallbackArg);

    void SetTargetTime(float targetTime) { m_targetTime = targetTime; }
    void SetLoop(bool bLoop) { m_bLoop = bLoop; }
    void SetTimerCallback(const Callback& timerCallback) { m_timerCallback = timerCallback; }
    void SetSwitch(bool bOn) { m_bOn = bOn; }

    float GetLocalTime() const { return m_localTime; }

private:
    float m_targetTime = 0.0f;
    float m_localTime = 0.0f;

    bool m_bOn = true;
    bool m_bLoop = false;
    
    CallbackArgs m_callbackArgs;
    Callback m_timerCallback = nullptr;
};