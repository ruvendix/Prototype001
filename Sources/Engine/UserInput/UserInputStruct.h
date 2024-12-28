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
	Timer timer; // Ȧ���� Ÿ�̸� �ʿ�, Ÿ�̸ӿ� �ݹ��� ����
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
	Timer timer; // Ȧ���� Ÿ�̸� �ʿ�, Ÿ�̸ӿ� �ݹ��� ����
};