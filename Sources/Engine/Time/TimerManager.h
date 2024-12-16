// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Timer.h"

// 크로노 리터럴을 편하게 사용
using namespace std::chrono_literals;

class TimerManager
{
    DECLARE_SINGLETON(TimerManager)

public:
    Timer CreateTimer(float targetTime, bool bRepetition);
    Timer CreateTimer(float targetTime, bool bRepetition, const TimerCallback& timerCallback);
    int32 RegisterTimer(float targetTime, bool bRepetition, const TimerCallback& timerCallback);

    void UpdateTimers(float deltaSeconds);
    void ModifyTimerRepetition(int32 timerId, bool bRepetition);

    Timer& FindTimer(int32 timerId);

private:
    int32 m_timerIdCounter = -1;
    std::unordered_map<int32, Timer> m_mapTimer;
};