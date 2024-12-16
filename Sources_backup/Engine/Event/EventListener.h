// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "EventId.h"
#include "EventMacro.h"

using EventArgs = std::vector<std::any>;

class EventListener : public IGameLoop
{
public:
	EventListener();
	virtual ~EventListener();

	virtual void ProcessEvent(EEventId eventId, const EventArgs& eventArgs) = 0;
};