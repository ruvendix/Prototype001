// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "InputAction.h"

InputAction::InputAction(const std::string& strInputActionName)
{
	m_strInputActionName = strInputActionName;
}

InputAction::~InputAction()
{

}

void InputAction::ModifyInputActionValue(bool bValue)
{
	m_inputActionValue.ApplyValue(bValue);
}

// 하나의 액션은 같은 핸들러를 사용하므로 같은 키값이 들어오면 안됨!
void InputAction::AddInputMappingInfo(const InputMappingInfo& inputMappingInfo)
{
	// 이미 등록된 키값이면 충돌나니까 패스
	auto foundIter = std::find_if(m_vecInputMappingInfo.cbegin(), m_vecInputMappingInfo.cend(),
		[&](const InputMappingInfo& elem)
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
	for (const InputMappingInfo& inputMappingInfo : m_vecInputMappingInfo)
	{
		// 키보드
		if (TO_NUM(inputMappingInfo.inputValue) < TO_NUM(EInputValue::KeyboardEnd))
		{
			if (KeyboardInputDevice::I()->CheckActivateInputTrigger(inputMappingInfo) == true)
			{
				m_inputActionHandler();
			}
		}
		// 마우스
		else
		{

		}
	}

	return bSucced;
}