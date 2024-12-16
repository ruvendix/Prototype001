// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "EventId.h"
#include "EventListener.h"

struct EventInfo
{
	EEventId eventId = EEventId::Unknown;
	EventArgs eventArgs;
	EventListener* pEventListener = nullptr;
};

class EventManager
{
	DECLARE_SINGLETON(EventManager)

public:
	void ProcessEvents();

	void TriggerEvent(EEventId eventId, EventListener* pEventListener);
	void TriggerEvent(EEventId eventId, EventArgs eventArgs, EventListener* pEventListener);

	void TriggerImmediatelyEvent(EEventId eventId, EventListener* pEventListener);
	void TriggerImmediatelyEvent(EEventId eventId, EventArgs eventArgs, EventListener* pEventListener);

private:
	std::queue<EventInfo> m_eventQueue;
};