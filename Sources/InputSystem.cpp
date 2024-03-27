/*
<내용 요약>
사용자의 입력을 관리하는 시스템
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
		// 입력값이 있는 상태
		if (currentInputStates[i] & 0x80)
		{
			// 최초로 눌린 상태
			if (m_inputStates[i] == EInputState::Up)
			{
				m_inputStates[i] = EInputState::Down;
			}
			// 전에도 눌린 상태
			else if (m_inputStates[i] == EInputState::Down)
			{
				m_inputStates[i] = EInputState::Press;
			}
		}
		// 입력값이 없는 상태
		else
		{
			// 전에 눌린 상태
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