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

// �ϳ��� �׼��� ���� �ڵ鷯�� ����ϹǷ� ���� Ű���� ������ �ȵ�!
void InputAction::AddInputMappingInfo(const InputActionMappingInfo& inputMappingInfo)
{
	// �̹� ��ϵ� Ű���̸� �浹���ϱ� �н�
	auto foundIter = std::find_if(m_vecInputMappingInfo.cbegin(), m_vecInputMappingInfo.cend(),
		[&](const InputActionMappingInfo& elem)
		{
			return (inputMappingInfo.inputValue == elem.inputValue);
		});

	// �̹� �ִ� Ű���̸� ��ȿ
	if (foundIter != m_vecInputMappingInfo.cend())
	{
		return;
	}

	m_vecInputMappingInfo.push_back(inputMappingInfo);
}

bool InputAction::ProcessInputAction()
{
	bool bSucced = false;

	// ���ε� ������ Ȯ���ϸ鼭 Ű�������� ���콺���� ���� => InputDevice�� ������
	for (const InputActionMappingInfo& inputMappingInfo : m_vecInputMappingInfo)
	{
		// Ʈ���ſ� ������ ���� ���� => � InputDevice�� ����ϴ��� �б� �ʿ�
		m_inputActionValue.ProcessInputActionValue(inputMappingInfo, m_inputActionValueType);

		// Ű����
		if (TO_NUM(inputMappingInfo.inputValue) < TO_NUM(EInputValue::KeyboardEnd))
		{
			if (CheckValidInputTriggerOnCurrentInputValueState(inputMappingInfo) == true)
			{
				m_inputActionHandler();
				bSucced = true;
			}
		}
		// ���콺
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