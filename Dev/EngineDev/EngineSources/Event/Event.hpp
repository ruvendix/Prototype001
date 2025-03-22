// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

template <typename T>
class Event;

template <typename... TArgs>
class Event<TArgs...>
{
public:
	template <typename TEventListener>
	void RegisterEventHandler(TEventListener* pEventListener, void(TEventListener::* pEventHandler)(TArgs...), TArgs&&... args)
	{
		m_delegator.AllDisconnectFunction();
		m_delegator.ConnectFixedArgumentsFunction(pEventListener, pEventHandler, std::forward<TArgs>(args)...);
	}

	void RegisterEventHandler(std::function<void(TArgs...)> eventHandler, TArgs&&... args)
	{
		m_delegator.AllDisconnectFunction();
		m_delegator.ConnectFixedArgumentsStaticFunction(eventHandler, std::forward<TArgs>(args)...);
	}

public:
	void ExcuteIfBound()
	{
		// 이벤트는 지연시킨 게 기본이라 인자를 미리 전달한 상태
		if (m_delegator.IsBoundAnyFixedArgumentsFunction() == false)
		{
			return;
		}

		m_delegator.InvokeFixedArgumentFunctions();
		m_delegator.AllDisconnectFunction();
	}

private:
	Delegator<void(TArgs&&)...> m_delegator;
};

template <>
class Event<void>
{
public:
	template <typename TEventListener>
	void RegisterEventHandler(TEventListener* pEventListener, void(TEventListener::* pEventHandler)())
	{
		m_delegator.AllDisconnectFunction();
		m_delegator.ConnectFixedArgumentsFunction(pEventListener, pEventHandler);
	}

	void RegisterEventHandler(std::function<void()> eventHandler)
	{
		m_delegator.AllDisconnectFunction();
		m_delegator.ConnectFixedArgumentsStaticFunction(eventHandler);
	}

public:
	void ExcuteIfBound()
	{
		// 이벤트는 지연시킨 게 기본이라 인자를 미리 전달한 상태
		if (m_delegator.IsBoundAnyFixedArgumentsFunction() == false)
		{
			return;
		}

		m_delegator.InvokeFixedArgumentFunctions();
		m_delegator.AllDisconnectFunction();
	}

private:
	DefaultDelegator m_delegator;
};