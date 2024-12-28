// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Event.h"
#include "EventListener.h"

struct EventInfo
{
	EventPtr spEvent = nullptr;
	CallbackArgs eventArgs;
};

class EventManager
{
	DECLARE_SINGLETON(EventManager)

public:
	void ProcessEvents();

	void ReserveEvent(const EventInfo& eventInfo);
	void TriggerEvent(const EventInfo& eventInfo);

private:
	std::queue<EventInfo> m_eventQueue;
};