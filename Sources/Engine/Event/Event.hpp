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
		m_delegator.ConnectFixedArgumentsFunction(pEventListener, pEventHandler, args...);
	}

	void ExcuteIfBound()
	{
		// �̺�Ʈ�� ������Ų �� �⺻�̶� ���ڸ� �̸� ������ ����
		if (m_delegator.IsBoundAnyFixedArgumentsFunction() == false)
		{
			return;
		}

		m_delegator.CallFixedArgumentFunctions();
		m_delegator.AllDisconnectFunction();
	}

private:
	Delegator<void(TArgs&&)...> m_delegator;
};