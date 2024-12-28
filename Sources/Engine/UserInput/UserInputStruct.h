// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

struct UserInputInfo
{
	int32 userInputVirtualCode = 0;
	EUserInputState userInputState = EUserInputState::Count;
};

struct KeyboardInputBoundInfo
{
	EKeyboardValue keyboardValue = EKeyboardValue::Count;
	std::array<Callback, TO_NUM(EUserInputState::Count)> arrCallback;
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
	std::array<Callback, TO_NUM(EUserInputState::Count)> arrCallback;
	CallbackArgs callbackArgs;
};

struct MouseHoldingInputBoundInfo
{
	EMouseValue mouseValue = EMouseValue::Count;
	Timer timer; // 홀딩은 타이머 필요, 타이머에 콜백이 있음
};