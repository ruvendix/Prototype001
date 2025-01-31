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