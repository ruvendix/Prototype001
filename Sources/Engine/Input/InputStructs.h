// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

struct InputMappingInfo
{
	EInputValue inputValue = EInputValue::Count;
	EInputTrigger inputTrigger = EInputTrigger::Count;
};

struct InputRawInfo
{
	int32 virtualCode = 0;
	EInputTrigger inputTrigger = EInputTrigger::Count;
};

struct InputMappingContextInfo
{
	int32 priority = 0;
	const InputMappingContextPtr spInputMappingContext = nullptr;

	// set�� �ְ� ����ϹǷ� ������ �����ε��� �߿�!
	bool operator<(const InputMappingContextInfo& other) const
	{
		return (priority < other.priority);
	}
};

struct KeyboardInputBoundInfo
{
	EKeyboardValue keyboardValue = EKeyboardValue::Count;
	std::array<Callback, TO_NUM(EInputTrigger::Count)> arrCallback;
	CallbackArgs callbackArgs;
};

struct KeyboardHoldingInputBoundInfo
{
	EKeyboardValue keyboardValue = EKeyboardValue::Count;
	Timer timer; // Ȧ���� Ÿ�̸� �ʿ�, Ÿ�̸ӿ� �ݹ��� ����
};

struct MouseInputBoundInfo
{
	EMouseValue mouseValue = EMouseValue::Count;
	std::array<Callback, TO_NUM(EInputTrigger::Count)> arrCallback;
	CallbackArgs callbackArgs;
};

struct MouseHoldingInputBoundInfo
{
	EMouseValue mouseValue = EMouseValue::Count;
	Timer timer; // Ȧ���� Ÿ�̸� �ʿ�, Ÿ�̸ӿ� �ݹ��� ����
};