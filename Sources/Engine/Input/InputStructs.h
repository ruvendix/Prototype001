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