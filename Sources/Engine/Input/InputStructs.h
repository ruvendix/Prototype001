// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

struct InputActionMappingInfo
{
	EInputValue inputValue = EInputValue::Unknown;
	EInputTrigger inputTrigger = EInputTrigger::OnlyPressed;
	InputActionValueModifierBitset inputActionValueModifierBitset;
};

struct InputRawInfo
{
	int32 virtualCode = 0;
	EInputValueState inputValueState = EInputValueState::Released;
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
	std::array<Callback, TO_NUM(EInputValueState::Count)> arrCallback;
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
	std::array<Callback, TO_NUM(EInputValueState::Count)> arrCallback;
	CallbackArgs callbackArgs;
};

struct MouseHoldingInputBoundInfo
{
	EMouseValue mouseValue = EMouseValue::Count;
	Timer timer; // Ȧ���� Ÿ�̸� �ʿ�, Ÿ�̸ӿ� �ݹ��� ����
};