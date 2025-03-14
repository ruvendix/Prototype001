// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "InputDeviceManager.h"

DEFINE_SINGLETON(InputDeviceMananger);

void InputDeviceMananger::Startup()
{
	InputRawInfo tempInputRawInfo;
	m_arrInputRawInfo.fill(tempInputRawInfo);

	tempInputRawInfo.virtualCode = VK_LEFT;
	m_arrInputRawInfo[TO_NUM(EInputValue::Left)] = tempInputRawInfo;

	tempInputRawInfo.virtualCode = VK_RIGHT;
	m_arrInputRawInfo[TO_NUM(EInputValue::Right)] = tempInputRawInfo;

	tempInputRawInfo.virtualCode = VK_DOWN;
	m_arrInputRawInfo[TO_NUM(EInputValue::Down)] = tempInputRawInfo;

	tempInputRawInfo.virtualCode = VK_UP;
	m_arrInputRawInfo[TO_NUM(EInputValue::Up)] = tempInputRawInfo;

	tempInputRawInfo.virtualCode = VK_SPACE;
	m_arrInputRawInfo[TO_NUM(EInputValue::SpaceBar)] = tempInputRawInfo;

	tempInputRawInfo.virtualCode = 'A';
	m_arrInputRawInfo[TO_NUM(EInputValue::A)] = tempInputRawInfo;

	tempInputRawInfo.virtualCode = VK_LBUTTON;
	m_arrInputRawInfo[TO_NUM(EInputValue::MouseLeftButton)] = tempInputRawInfo;

	tempInputRawInfo.virtualCode = VK_RBUTTON;
	m_arrInputRawInfo[TO_NUM(EInputValue::MouseRightButton)] = tempInputRawInfo;

	m_spKeyboardDevice = std::make_shared<KeyboardDevice>();
	m_spMouseDevice = std::make_shared<MouseDevice>();
}

bool InputDeviceMananger::Update(float deltaSeconds)
{
	BYTE arrKeyState[256] = { };
	if (::GetKeyboardState(arrKeyState) == FALSE)
	{
		return false;
	}

	// 플레이어의 현재 입력 상태를 확인
	int32 inputValueCount = TO_NUM(EInputValue::Count);
	for (int32 i = 0; i < inputValueCount; ++i)
	{
		InputRawInfo& refInputRawInfo = m_arrInputRawInfo[i];

		/*
		이전 프레임에서는 누르지 않았지만 현재 프레임에서는 누른 경우와
		이전 프레임에서 눌렀는데 현재 프레임에서도 누른 경우일 때
		최상위 비트는 1이 되므로 0x80으로 확인 가능
		*/
		bool bExistedLocalPlayerInput = ((arrKeyState[refInputRawInfo.virtualCode] & 0x80) != 0);
		switch (refInputRawInfo.inputValueState)
		{
		case EInputValueState::Pressed:
		{
			if (bExistedLocalPlayerInput == true)
			{
				refInputRawInfo.inputValueState = EInputValueState::Holding;
			}
			else
			{
				refInputRawInfo.inputValueState = EInputValueState::Released;
			}

			break;
		}

		case EInputValueState::Holding:
		{
			if (bExistedLocalPlayerInput == false)
			{
				refInputRawInfo.inputValueState = EInputValueState::Released;
			}

			break;
		}

		case EInputValueState::Released:
		{
			if (bExistedLocalPlayerInput == true)
			{
				refInputRawInfo.inputValueState = EInputValueState::Pressed;
			}
			else
			{
				refInputRawInfo.inputValueState = EInputValueState::None;
			}

			break;
		}

		case EInputValueState::None:
		{
			if (bExistedLocalPlayerInput == true)
			{
				refInputRawInfo.inputValueState = EInputValueState::Pressed;
			}

			break;
		}
		}
	}

	// 마우스 좌표 갱신
	if (m_spMouseDevice != nullptr)
	{
		m_spMouseDevice->Update(deltaSeconds);
	}

	return true;
}