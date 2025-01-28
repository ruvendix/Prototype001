// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "InputAction.h"

namespace
{
	bool CheckValidInputTriggerOnCurrentInputValueState(const InputActionMappingInfo& inputMappingInfo)
	{
		bool bResult = false;
		EInputValueState currentInputValueState = KeyboardInputDevice::I()->GetCurrentInputValueState(inputMappingInfo.inputValue);

		switch (inputMappingInfo.inputTrigger)
		{
		case EInputTrigger::OnlyPressed:
		{
			if (currentInputValueState == EInputValueState::Pressed)
			{
				bResult = true;
			}
		}
		break;

		case EInputTrigger::PressedAndRelease:
		{
			if ((currentInputValueState == EInputValueState::Pressed) ||
				(currentInputValueState == EInputValueState::Released))
			{
				bResult = true;
			}
		}
		break;

		case EInputTrigger::OnlyReleased:
		{
			bResult = (currentInputValueState == EInputValueState::Released);
		}
		break;

		case EInputTrigger::Triggered:
		{
			bResult = true;
		}
		break;

		default: break;
		}

		return bResult;
	}
}

InputAction::InputAction(const std::string& strInputActionName)
{
	m_strInputActionName = strInputActionName;
}

InputAction::~InputAction()
{

}

// 하나의 액션은 같은 핸들러를 사용하므로 같은 키값이 들어오면 안됨!
void InputAction::AddInputMappingInfo(const InputActionMappingInfo& inputMappingInfo)
{
	// 이미 등록된 키값이면 충돌나니까 패스
	auto foundIter = std::find_if(m_vecInputMappingInfo.cbegin(), m_vecInputMappingInfo.cend(),
		[&](const InputActionMappingInfo& elem)
		{
			return (inputMappingInfo.inputValue == elem.inputValue);
		});

	// 이미 있는 키값이면 무효
	if (foundIter != m_vecInputMappingInfo.cend())
	{
		return;
	}

	m_vecInputMappingInfo.push_back(inputMappingInfo);
}

bool InputAction::ProcessInputAction()
{
	bool bSucced = false;

	// 매핑된 정보를 확인하면서 키보드인지 마우스인지 구분 => InputDevice의 다형성
	for (const InputActionMappingInfo& inputMappingInfo : m_vecInputMappingInfo)
	{
		// 트리거와 별개로 값을 세팅 => 어떤 InputDevice를 사용하는지 분기 필요
		m_inputActionValue.ProcessInputActionValue(inputMappingInfo, m_inputActionValueType);

		// 키보드
		if (TO_NUM(inputMappingInfo.inputValue) < TO_NUM(EInputValue::KeyboardEnd))
		{
			if (CheckValidInputTriggerOnCurrentInputValueState(inputMappingInfo) == true)
			{
				m_inputActionHandler();
				bSucced = true;
			}
		}
		// 마우스
		else
		{

		}
	}

	return bSucced;
}

void InputAction::ApplyInputActionValue(const InputActionValue& inputActionValue, EInputActionValueType inputActionValueType)
{
	m_inputActionValue = inputActionValue;
	m_inputActionValueType = inputActionValueType;
}