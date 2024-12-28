// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#define DEFINE_EVENT_HANDLER \
public: \
	template <typename TEvent> \
	Callback FindEventHandler() \
	{ \
		const auto& foundIter = m_mapEventHandler.find(TEvent::s_id); \
		if (foundIter == m_mapEventHandler.cend()) \
		{ \
			return nullptr; \
		} \
	\
		return (foundIter->second); \
	} \
	\
	template <typename TEvent, typename TEventHandler> \
	void RegisterEventHandler(TEventHandler eventHandler) \
	{ \
		ASSERT_LOG_RETURN(FindEventHandler<TEvent>() == nullptr); \
	\
		Callback eventHandlerObj = std::bind(eventHandler, this, std::placeholders::_1); \
		m_mapEventHandler.insert(std::make_pair(TEvent::s_id, eventHandlerObj)); \
	} \
	\
	template <typename TEvent, typename... TArgs> \
	void ReserveEvent(TArgs... args) \
	{ \
		Callback foundEventHandler = FindEventHandler<TEvent>(); \
		ASSERT_LOG_RETURN(foundEventHandler != nullptr); \
	\
		EventPtr spEvent = std::make_shared<TEvent>(); \
		spEvent->SetEventHandler(foundEventHandler); \
	\
		EventInfo eventInfo; \
		eventInfo.spEvent = std::move(spEvent); \
		eventInfo.eventArgs = { std::forward<TArgs>(args)... }; \
	\
		EventManager::I()->ReserveEvent(eventInfo); \
	} \
\
private: \
	std::unordered_map<int32, Callback> m_mapEventHandler