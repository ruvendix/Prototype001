/*
<���� ���>
������� �Է��� �����ϴ� �ý���
*/

#include "Pch.h"
#include "InputSystem.h"

#include "GameApplication.h"

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::Startup()
{
	m_inputStates.fill(EInputState::Up);
}

bool InputSystem::Update()
{
	const uint32 inputValueCount = ENUM_TO_NUM(EInputValue::Count);
	std::array<BYTE, inputValueCount> currentInputStates;
	if (::GetKeyboardState(currentInputStates.data()) == FALSE)
	{
		return false;
	}

	for (uint32 i = 0; i < inputValueCount; ++i)
	{
		// �Է°��� �ִ� ����
		if (currentInputStates[i] & 0x80)
		{
			// ���ʷ� ���� ����
			if (m_inputStates[i] == EInputState::Up)
			{
				m_inputStates[i] = EInputState::Down;
			}
			// ������ ���� ����
			else if (m_inputStates[i] == EInputState::Down)
			{
				m_inputStates[i] = EInputState::Press;
			}
		}
		// �Է°��� ���� ����
		else
		{
			// ���� ���� ����
			if ((m_inputStates[i] == EInputState::Down) ||
				(m_inputStates[i] == EInputState::Press))
			{
				m_inputStates[i] = EInputState::Up;
			}
		}
	}

	POINT tempPos;
	if ((::GetCursorPos(&tempPos) == FALSE) ||
		(::ScreenToClient(GameApplication::I()->GetApplicationWindowHandle(), &tempPos) == FALSE))
	{
		return false;
	}

	m_mousePos.x = static_cast<int32>(tempPos.x);
	m_mousePos.y = static_cast<int32>(tempPos.y);

	return true;
}