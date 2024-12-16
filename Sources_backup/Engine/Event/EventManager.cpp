// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "EventManager.h"

DEFINE_SINGLETON(EventManager);

void EventManager::ProcessEvents()
{
	while (m_eventQueue.empty() == false)
	{
		const EventInfo& eventInfo = m_eventQueue.front();
		EventListener* pEventListener = eventInfo.pEventListener;
		if (pEventListener == nullptr)
		{
			continue;
		}
		pEventListener->ProcessEvent(eventInfo.eventId, eventInfo.eventArgs);
		m_eventQueue.pop();
	}
}

void EventManager::TriggerEvent(EEventId eventId, EventListener* pEventListener)
{
	EventArgs nullEventArgs;
	TriggerEvent(eventId, nullEventArgs, pEventListener);
}

void EventManager::TriggerEvent(EEventId eventId, EventArgs eventArgs, EventListener* pEventListener)
{
	EventInfo eventInfo;
	eventInfo.eventId = eventId;
	eventInfo.eventArgs = std::move(eventArgs);
	eventInfo.pEventListener = pEventListener;

	m_eventQueue.push(eventInfo);
}

void EventManager::TriggerImmediatelyEvent(EEventId eventId, EventListener* pEventListener)
{
	EventArgs nullEventArgs;
	TriggerImmediatelyEvent(eventId, nullEventArgs, pEventListener);
}

void EventManager::TriggerImmediatelyEvent(EEventId eventId, EventArgs eventArgs, EventListener* pEventListener)
{
	if (pEventListener == nullptr)
	{
		return;
	}

	pEventListener->ProcessEvent(eventId, eventArgs);
}