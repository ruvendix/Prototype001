// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "InputActionValue.h"

#include "Engine/UtilityClass/EnumBitset.hpp"

DEFINE_LOG_CATEGORY(LogInput);

void InputActionValue::ProcessInputActionValue(const InputActionMappingInfo& inputActionMappingInfo, EInputActionValueType inputActionValueType)
{
	const InputActionValueModifierBitset& inputActionValueModifierBitset = inputActionMappingInfo.inputActionValueModifierBitset;
	EInputValueState currentInputValueState = KeyboardInputDevice::I()->GetCurrentInputValueState(inputActionMappingInfo.inputValue);
	
	bool bActivationValue = false;
	if ((currentInputValueState == EInputValueState::Pressed) ||
		(currentInputValueState == EInputValueState::Holding))
	{
		bActivationValue = true;
	}

	switch (inputActionValueType)
	{
	case EInputActionValueType::Boolean:
	{
		m_value = (bActivationValue == true);
	}
	break;

	case EInputActionValueType::Vector1:
	{
		if (bActivationValue == true)
		{
			if (inputActionValueModifierBitset.IsBitOn(EInputActionValueModifierType::Negative))
			{
				m_value = -1.0f;
			}
			else
			{
				m_value = 1.0f;
			}
		}
		else
		{
			m_value = 0.0f;
		}
	}
	break;

	case EInputActionValueType::Vector2:

		break;

	case EInputActionValueType::Vector3:

		break;

	default: break;
	}
}