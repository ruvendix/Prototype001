// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#define DECLARE_EVENT_HANDLER(TEvent) \
public: \
	using EventHandler = void (TEvent::*)(const EventArgs&); \
\
	virtual void ProcessEvent(EEventId eventId, const EventArgs& eventArgs) override; \
	EventHandler FindEventHander(EEventId eventId); \
	void RegisterEventHandler(EEventId eventId, EventHandler eventHandler); \
	void TriggerEvent(EEventId eventId, const EventArgs& eventArgs); \
\
private: \
	std::unordered_map<EEventId, EventHandler> m_mapEventHandler;

#define DEFINE_EVENT_HANDLER(TEvent) \
TEvent::EventHandler TEvent::FindEventHander(EEventId eventId) \
{ \
	const auto& foundIter = m_mapEventHandler.find(eventId); \
	if (foundIter == m_mapEventHandler.cend()) \
	{ \
		return nullptr; \
	} \
	return (foundIter->second); \
} \
\
void TEvent::RegisterEventHandler(EEventId eventId, TEvent::EventHandler eventHandler) \
{ \
	if (FindEventHander(eventId) != nullptr) \
	{ \
		return; \
	} \
 \
	const auto& retIter = m_mapEventHandler.insert(std::make_pair(eventId, eventHandler)); \
	ASSERT_LOG(retIter.second == true); \
} \
\
void TEvent::ProcessEvent(EEventId eventId, const EventArgs& eventArgs) \
{ \
	TEvent::EventHandler eventHandler = FindEventHander(eventId); \
	ASSERT_LOG(eventHandler != nullptr); \
	(this->*eventHandler)(eventArgs); \
} \
\
void TEvent::TriggerEvent(EEventId eventId, const EventArgs& eventArgs) \
{ \
	EventManager::I()->TriggerEvent(eventId, eventArgs, this); \
}