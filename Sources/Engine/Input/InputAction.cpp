// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "InputAction.h"

namespace
{
	bool CheckValidInputTriggerOnCurrentInputValueState(const InputActionMappingInfo& inputMappingInfo)
	{
		bool bResult = false;
		EInputValueState currentInputValueState = InputDeviceMananger::I()->GetCurrentInputValueState(inputMappingInfo.inputValue);

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

		case EInputTrigger::PressedAndHolding:
		{
			if ((currentInputValueState == EInputValueState::Pressed) ||
				(currentInputValueState == EInputValueState::Holding))
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
	auto foundIter = std::find_if(m_vecInputActionMappingInfo.cbegin(), m_vecInputActionMappingInfo.cend(),
		[&](const InputActionMappingInfo& elem)
		{
			return (inputMappingInfo.inputValue == elem.inputValue);
		});

	// 이미 있는 키값이면 무효
	if (foundIter != m_vecInputActionMappingInfo.cend())
	{
		return;
	}

	m_vecInputActionMappingInfo.push_back(inputMappingInfo);
}

bool InputAction::ProcessInputAction()
{
	bool bSucced = false;

	// 매핑된 정보를 확인하면서 입력 내용 처리
	for (const InputActionMappingInfo& inputActionMappingInfo : m_vecInputActionMappingInfo)
	{
		// 트리거와 별개로 값을 세팅
		m_inputActionValue.ProcessInputActionValue(inputActionMappingInfo, m_inputActionValueType);

		// 현재 입력 상태가 설정한 트리거를 작동시키는지?
		if (CheckValidInputTriggerOnCurrentInputValueState(inputActionMappingInfo) == true)
		{
			bSucced = true;
		}
	}

	if (bSucced == true) // 일반적으로는 키 하나당 매핑 정보가 하나인데 축 입력 같은 경우는 키 여러개에 함수는 하나
	{
		m_inputActionDelegator.InvokeFixedArgumentFunctions();
	}

	m_inputActionValue.ResetInputActionValue();
	return bSucced;
}

void InputAction::ApplyInputActionValue(const InputActionValue& inputActionValue, EInputActionValueType inputActionValueType)
{
	m_inputActionValue = inputActionValue;
	m_inputActionValueType = inputActionValueType;
}