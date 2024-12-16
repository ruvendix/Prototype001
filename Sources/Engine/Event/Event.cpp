// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Event.h"

Event::Event()
{

}

Event::~Event()
{

}

void Event::ProcessEvent(const EventArgs& eventArgs)
{
	m_eventHandler(eventArgs);
}