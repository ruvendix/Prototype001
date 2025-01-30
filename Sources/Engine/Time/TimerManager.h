// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Timer.h"

// 크로노 리터럴을 편하게 사용
using namespace std::chrono_literals;

class TimerManager : public ICoreLoop
{
    DECLARE_SINGLETON(TimerManager)

public:
    template <typename TObject, typename... TArgs>
    Timer CreateTimer(float triggerTime, bool bRepetition, TObject* pObj, void(TObject::* memFunc)(TArgs...), TArgs&&... args)
    {
        Timer timer;
        timer.SetTriggerTime(triggerTime);
        timer.SetLoop(bRepetition);

        TimerCallback timerCallback = std::bind(memFunc, pObj, std::forward<TArgs>(args)...);
        timer.SetTimerCallback(timerCallback);

        return timer;
    }

    template <typename TObject, typename... TArgs>
    int32 RegisterTimer(float triggerTime, bool bRepetition, TObject* pObj, TArgs&&... args)
    {
        Timer newTimer = CreateTimer(triggerTime, bRepetition, pObj, std::forward<TArgs>(args)...);

        ++m_timerIdCounter; // 등록할 타이머의 Id
        const auto& resultInsert = m_mapTimer.insert(std::make_pair(m_timerIdCounter, newTimer));
        if (resultInsert.second == false)
        {
            ERROR_LOG(LogTimer, "타이머 등록 실패!");
        }

        return m_timerIdCounter;
    }

public:
    Timer CreateTimer(float targetTime, bool bRepetition);
    int32 RegisterTimer(float targetTime, bool bRepetition);

    void UpdateTimers(float deltaSeconds);
    void ModifyTimerRepetition(int32 timerId, bool bRepetition);

    Timer& FindTimer(int32 timerId);

private:
    int32 m_timerIdCounter = -1;
    std::unordered_map<int32, Timer> m_mapTimer;
};