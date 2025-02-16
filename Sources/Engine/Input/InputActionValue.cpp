// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "InputActionValue.h"

#include "Engine/UtilityClass/EnumBitset.hpp"

DEFINE_LOG_CATEGORY(LogInput);

void InputActionValue::ProcessInputActionValue(const InputActionMappingInfo& inputActionMappingInfo, EInputActionValueType inputActionValueType)
{
	const InputActionValueModifierBitset& inputActionValueModifierBitset = inputActionMappingInfo.inputActionValueModifierBitset;
	EInputValueState currentInputValueState = InputDeviceMananger::I()->GetCurrentInputValueState(inputActionMappingInfo.inputValue);
	
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
		float resultValue = 0.0f;

		if (bActivationValue == true)
		{
			resultValue = 1.0f;

			if (inputActionValueModifierBitset.IsBitOn(EInputActionValueModifierType::Negative))
			{
				resultValue *= (-1.0f);
			}
		}

		m_value = resultValue;
	}
	break;

	case EInputActionValueType::Vector2:
	{
		Vector2d vAxis2d;

		if (m_value.has_value() == true)
		{
			vAxis2d = std::any_cast<Vector2d>(m_value);
		}

		if (bActivationValue == true)
		{
			if (inputActionValueModifierBitset.IsBitOn(EInputActionValueModifierType::Swizzle))
			{
				vAxis2d.y += 1.0f;

				if (inputActionValueModifierBitset.IsBitOn(EInputActionValueModifierType::Negative))
				{
					vAxis2d.y *= (-1.0f);
				}
			}
			else
			{
				vAxis2d.x += 1.0f;

				if (inputActionValueModifierBitset.IsBitOn(EInputActionValueModifierType::Negative))
				{
					vAxis2d.x *= (-1.0f);
				}
			}
		}

		m_value = vAxis2d;
		//DEFAULT_TRACE_LOG("(%f %f) %d", vAxis2d.x, vAxis2d.y, TO_NUM(inputActionMappingInfo.inputValue));
	}
	break;

	case EInputActionValueType::Vector3:

		break;

	default: break;
	}
}

void InputActionValue::ResetInputActionValue()
{
	m_value.reset();
}