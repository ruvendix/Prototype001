// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    void Update(float deltaSeconds);
    void Reset();

    void SetOn() { m_bOn = true; }
    void SetOff() { m_bOn = false; }

    void SetTriggerTime(float triggerTime) { m_triggerTime = triggerTime; }
    void SetLoop(bool bLoop) { m_bLoop = bLoop; }
    void SetSwitch(bool bOn) { m_bOn = bOn; }
    void SetTimerDelegator(const DefaultDelegator& timerDelegator) { m_timerDelegator = timerDelegator; }

    float GetLocalTime() const { return m_localTime; }
    bool IsTimerOn() const { return (m_bOn == true); }

private:
    float m_triggerTime = 0.0f;
    float m_localTime = 0.0f;

    bool m_bOn = true;
    bool m_bLoop = false;

    DefaultDelegator m_timerDelegator;
};