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

// �ϳ��� �׼��� ���� �ڵ鷯�� ����ϹǷ� ���� Ű���� ������ �ȵ�!
void InputAction::AddInputMappingInfo(const InputActionMappingInfo& inputMappingInfo)
{
	// �̹� ��ϵ� Ű���̸� �浹���ϱ� �н�
	auto foundIter = std::find_if(m_vecInputActionMappingInfo.cbegin(), m_vecInputActionMappingInfo.cend(),
		[&](const InputActionMappingInfo& elem)
		{
			return (inputMappingInfo.inputValue == elem.inputValue);
		});

	// �̹� �ִ� Ű���̸� ��ȿ
	if (foundIter != m_vecInputActionMappingInfo.cend())
	{
		return;
	}

	m_vecInputActionMappingInfo.push_back(inputMappingInfo);
}

bool InputAction::ProcessInputAction()
{
	bool bSucced = false;

	// ���ε� ������ Ȯ���ϸ鼭 �Է� ���� ó��
	for (const InputActionMappingInfo& inputActionMappingInfo : m_vecInputActionMappingInfo)
	{
		// Ʈ���ſ� ������ ���� ����
		m_inputActionValue.ProcessInputActionValue(inputActionMappingInfo, m_inputActionValueType);

		// ���� �Է� ���°� ������ Ʈ���Ÿ� �۵���Ű����?
		if (CheckValidInputTriggerOnCurrentInputValueState(inputActionMappingInfo) == true)
		{
			bSucced = true;
		}
	}

	if (bSucced == true) // �Ϲ������δ� Ű �ϳ��� ���� ������ �ϳ��ε� �� �Է� ���� ���� Ű �������� �Լ��� �ϳ�
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