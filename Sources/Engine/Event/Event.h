// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EEventId
{
	Unknown = 0,
	ChangeScene,
	ChangeState,
};

DEFINE_COMPILETIMER_COUNTER(EventIdCounter)

class Event
{
public:
	Event();
	virtual ~Event();

	virtual void ProcessEvent(const EventArgs& eventArgs);

	void SetEventHandler(const EventHandler& eventHandler) { m_eventHandler = eventHandler; }

protected:
	EventHandler m_eventHandler = nullptr;
};