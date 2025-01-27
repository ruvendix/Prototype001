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

	// set에 넣고 사용하므로 연산자 오버로딩이 중요!
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
	Timer timer; // 홀딩은 타이머 필요, 타이머에 콜백이 있음
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
	Timer timer; // 홀딩은 타이머 필요, 타이머에 콜백이 있음
};