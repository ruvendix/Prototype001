// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "KeyboardInputHandler.h"

DEFINE_SINGLETON(KeyboardInputHandler);

/*
이전 프레임에서는 누르지 않았지만 현재 프레임에서는 누른 경우와
이전 프레임에서 눌렀는데 현재 프레임에서도 누른 경우일 때
최상위 비트는 1이 되므로 0x8000으로 확인 가능 */
#define IS_EXIST_USER_INPUT(virtualKeyCode) ((::GetAsyncKeyState(virtualKeyCode) & 0x8000) != 0)

void KeyboardInputHandler::Startup()
{
	// 초기화
	UserInputInfo userInputInfo;
	m_arrCurrentKeyboardUserInputInfo.fill(userInputInfo);

	userInputInfo.userInputVirtualCode = VK_LEFT;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Left)] = userInputInfo;

	userInputInfo.userInputVirtualCode = VK_RIGHT;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Right)] = userInputInfo;

	userInputInfo.userInputVirtualCode = VK_DOWN;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Down)] = userInputInfo;

	userInputInfo.userInputVirtualCode = VK_UP;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Up)] = userInputInfo;

	userInputInfo.userInputVirtualCode = 'A';
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::A)] = userInputInfo;
}

bool KeyboardInputHandler::Update(float deltaSeconds)
{
	UNREFERENCED_PARAMETER(deltaSeconds);

	// 유저의 현재 키보드 입력 상태를 확인
	int32 keyboardValueCount = TO_NUM(EKeyboardValue::Count);
	for (int32 i = 0; i < keyboardValueCount; ++i)
	{
		UserInputInfo& refUserInputInfo = m_arrCurrentKeyboardUserInputInfo[i];
		bool bExistUserInput = IS_EXIST_USER_INPUT(refUserInputInfo.userInputVirtualCode);
		
		// 일단 해보고 상태 패턴으로 전환
		switch (refUserInputInfo.userInputState)
		{
		case EUserInputState::Down:
		{
			if (bExistUserInput == true)
			{
				refUserInputInfo.userInputState = EUserInputState::Pressing;
			}
			else
			{
				refUserInputInfo.userInputState = EUserInputState::Up;
			}

			break;
		}

		case EUserInputState::Pressing:
		{
			if (bExistUserInput == false)
			{
				refUserInputInfo.userInputState = EUserInputState::Up;
			}

			break;
		}

		case EUserInputState::Up:
		{
			if (bExistUserInput == true)
			{
				refUserInputInfo.userInputState = EUserInputState::Down;
			}
			else
			{
				refUserInputInfo.userInputState = EUserInputState::Count;
			}

			break;
		}

		// Down으로 전환용
		case EUserInputState::Count:
		{
			if (bExistUserInput == true)
			{
				refUserInputInfo.userInputState = EUserInputState::Down;
			}

			break;
		}
		}
	}

	for (const auto& iter : m_mapKeyboardInputBoundInfo)
	{
		EUserInputState userInputState = m_arrCurrentKeyboardUserInputInfo[TO_NUM(iter.first)].userInputState;
		if (userInputState == EUserInputState::Count)
		{
			continue;
		}

		const UserInputCallback& userInputCallback = iter.second.arrCallback[TO_NUM(userInputState)];
		if (userInputCallback != nullptr)
		{
			userInputCallback();
		}
	}

	for (auto& refIter : m_mapKeyboardHoldingInputBoundInfo)
	{
		EUserInputState userInputState = m_arrCurrentKeyboardUserInputInfo[TO_NUM(refIter.first)].userInputState;
		if (userInputState == EUserInputState::Count)
		{
			continue;
		}

		// 홀딩일 때만 타이머 증가
		if (userInputState == EUserInputState::Pressing)
		{
			refIter.second.timer.Update(deltaSeconds);
		}
		else if (userInputState == EUserInputState::Up)
		{
			refIter.second.timer.SetSwitch(true);
		}
		else
		{
			refIter.second.timer.Reset();
		}
	}

	return true;
}

void KeyboardInputHandler::BindKeyboardInput(EKeyboardValue keyboardValue,
	const UserInputCallback& downCallback, const UserInputCallback& pressingCallback, const UserInputCallback& upCallback)
{
	if (m_mapKeyboardInputBoundInfo.find(keyboardValue) != m_mapKeyboardInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "이미 존재하는 바인드 정보!");
		return;
	}

	KeyboardInputBoundInfo keyboardInputBoundInfo;
	keyboardInputBoundInfo.keyboardValue = keyboardValue;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Down)] = downCallback;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Pressing)] = pressingCallback;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Up)] = upCallback;

	const auto& retIter = m_mapKeyboardInputBoundInfo.insert(std::make_pair(keyboardValue, keyboardInputBoundInfo));
	ASSERT_LOG(retIter.second == true);
}

void KeyboardInputHandler::BindKeyboardHoldingInput(EKeyboardValue keyboardValue, float targetTime, const UserInputCallback& holdingEndCallback)
{
	if (m_mapKeyboardHoldingInputBoundInfo.find(keyboardValue) != m_mapKeyboardHoldingInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "이미 존재하는 홀딩 바인드 정보!");
		return;
	}

	KeyboardHoldingInputBoundInfo keyboardHoldingInputBoundInfo;
	keyboardHoldingInputBoundInfo.keyboardValue = keyboardValue;
	keyboardHoldingInputBoundInfo.timer = TimerManager::I()->CreateTimer(targetTime, false, holdingEndCallback);

	const auto& retIter = m_mapKeyboardHoldingInputBoundInfo.insert(std::make_pair(keyboardValue, keyboardHoldingInputBoundInfo));
	ASSERT_LOG(retIter.second == true);
}

bool KeyboardInputHandler::CheckKeyboardValueDown(EKeyboardValue keyboardValue) const
{
	return (m_arrCurrentKeyboardUserInputInfo[TO_NUM(keyboardValue)].userInputState == EUserInputState::Down);
}

bool KeyboardInputHandler::CheckKeyboardValuePressing(EKeyboardValue keyboardValue) const
{
	return (m_arrCurrentKeyboardUserInputInfo[TO_NUM(keyboardValue)].userInputState == EUserInputState::Pressing);
}

bool KeyboardInputHandler::CheckKeyboardValueUp(EKeyboardValue keyboardValue) const
{
	return (m_arrCurrentKeyboardUserInputInfo[TO_NUM(keyboardValue)].userInputState == EUserInputState::Up);
}
