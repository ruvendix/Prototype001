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

// �ϳ��� �׼��� ���� �ڵ鷯�� ����ϹǷ� ���� Ű���� ������ �ȵ�!
void InputAction::AddInputMappingInfo(const InputMappingInfo& inputMappingInfo)
{
	// �̹� ��ϵ� Ű���̸� �浹���ϱ� �н�
	auto foundIter = std::find_if(m_vecInputMappingInfo.cbegin(), m_vecInputMappingInfo.cend(),
		[&](const InputMappingInfo& elem)
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
	for (const InputMappingInfo& inputMappingInfo : m_vecInputMappingInfo)
	{
		// Ű����
		if (TO_NUM(inputMappingInfo.inputValue) < TO_NUM(EInputValue::KeyboardEnd))
		{
			if (KeyboardInputDevice::I()->CheckActivateInputTrigger(inputMappingInfo) == true)
			{
				m_inputActionHandler();
			}
		}
		// ���콺
		else
		{

		}
	}

	return bSucced;
}