// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "TimerManager.h"

DEFINE_SINGLETON(TimerManager);

Timer TimerManager::CreateTimer(float targetTime, bool bRepetition)
{
    return CreateTimer(targetTime, bRepetition, nullptr);
}

Timer TimerManager::CreateTimer(float targetTime, bool bRepetition, const TimerCallback& timerCallback)
{
    Timer timer;
    timer.SetTargetTime(targetTime);
    timer.SetLoop(bRepetition);
    timer.SetTimerCallback(timerCallback);

    return timer;
}

int32 TimerManager::RegisterTimer(float targetTime, bool bRepetition, const TimerCallback& timerCallback)
{
    Timer timer = CreateTimer(targetTime, bRepetition, timerCallback);

    ++m_timerIdCounter; // 등록할 타이머의 Id
    const auto& insertRet = m_mapTimer.insert(std::make_pair(m_timerIdCounter, timer));
    if (insertRet.second == false)
    {
        ERROR_LOG(LogTimer, "타이머 등록 실패!");
    }

    return m_timerIdCounter;
}

void TimerManager::UpdateTimers(float deltaSeconds)
{
    for (auto& refIter : m_mapTimer)
    {
        Timer& refTimer = (refIter.second);
        refTimer.Update(deltaSeconds);
    }
}

void TimerManager::ModifyTimerRepetition(int32 timerId, bool bRepetition)
{
    FindTimer(timerId).SetLoop(bRepetition);
}

Timer& TimerManager::FindTimer(int32 timerId)
{
    const auto& foundIter = m_mapTimer.find(timerId);
    ASSERT_LOG(foundIter != m_mapTimer.cend());
    return (foundIter->second);
}