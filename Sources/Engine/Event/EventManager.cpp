// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EventManager.h"

DEFINE_SINGLETON(EventManager);

void EventManager::ProcessEvents()
{
	while (m_eventQueue.empty() == false)
	{
		const EventInfo& eventInfo = m_eventQueue.front();

		Event* pEvent = eventInfo.spEvent.get();
		if (pEvent == nullptr)
		{
			continue;
		}

		pEvent->ProcessEvent(eventInfo.eventArgs);
		m_eventQueue.pop();
	}
}

void EventManager::ReserveEvent(const EventInfo& eventInfo)
{
	m_eventQueue.push(eventInfo);
}

void EventManager::TriggerEvent(const EventInfo& eventInfo)
{
	Event* pEvent = eventInfo.spEvent.get();
	ASSERT_LOG_RETURN(pEvent != nullptr);
	pEvent->ProcessEvent(eventInfo.eventArgs);
}