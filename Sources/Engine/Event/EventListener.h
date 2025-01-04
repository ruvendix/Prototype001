// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Event.h"
#include "EventMacro.h"

class EventListener : public BaseElement
{
public:
	EventListener();
	virtual ~EventListener();

	virtual void ProcessEvent(EEventId eventId, const CallbackArgs& eventArgs) = 0;
};