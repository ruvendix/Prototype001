// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "KeyboardInputHandler.h"

#include "UserInputMacro.h"

DEFINE_SINGLETON(KeyboardInputHandler);

void KeyboardInputHandler::Startup()
{
	// �ʱ�ȭ
	UserInputInfo keyboardInputInfo;
	m_arrCurrentKeyboardUserInputInfo.fill(keyboardInputInfo);

	keyboardInputInfo.userInputVirtualCode = VK_LEFT;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Left)] = keyboardInputInfo;

	keyboardInputInfo.userInputVirtualCode = VK_RIGHT;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Right)] = keyboardInputInfo;

	keyboardInputInfo.userInputVirtualCode = VK_DOWN;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Down)] = keyboardInputInfo;

	keyboardInputInfo.userInputVirtualCode = VK_UP;
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::Up)] = keyboardInputInfo;

	keyboardInputInfo.userInputVirtualCode = 'A';
	m_arrCurrentKeyboardUserInputInfo[TO_NUM(EKeyboardValue::A)] = keyboardInputInfo;
}

bool KeyboardInputHandler::Update(float deltaSeconds)
{
	UNREFERENCED_PARAMETER(deltaSeconds);

	// ������ ���� Ű���� �Է� ���¸� Ȯ��
	int32 keyboardValueCount = TO_NUM(EKeyboardValue::Count);
	for (int32 i = 0; i < keyboardValueCount; ++i)
	{
		UserInputInfo& refKeyboardInputInfo = m_arrCurrentKeyboardUserInputInfo[i];
		bool bExistKeyboardInput = IS_EXIST_USER_INPUT(refKeyboardInputInfo.userInputVirtualCode);
		
		switch (refKeyboardInputInfo.userInputState)
		{
		case EUserInputState::Down:
		{
			if (bExistKeyboardInput == true)
			{
				refKeyboardInputInfo.userInputState = EUserInputState::Pressing;
			}
			else
			{
				refKeyboardInputInfo.userInputState = EUserInputState::Up;
			}

			break;
		}

		case EUserInputState::Pressing:
		{
			if (bExistKeyboardInput == false)
			{
				refKeyboardInputInfo.userInputState = EUserInputState::Up;
			}

			break;
		}

		case EUserInputState::Up:
		{
			if (bExistKeyboardInput == true)
			{
				refKeyboardInputInfo.userInputState = EUserInputState::Down;
			}
			else
			{
				refKeyboardInputInfo.userInputState = EUserInputState::Count;
			}

			break;
		}

		// Down���� ��ȯ��
		case EUserInputState::Count:
		{
			if (bExistKeyboardInput == true)
			{
				refKeyboardInputInfo.userInputState = EUserInputState::Down;
			}

			break;
		}
		}
	}

	for (const auto& iter : m_mapKeyboardInputBoundInfo)
	{
		EUserInputState keyboardInputState = m_arrCurrentKeyboardUserInputInfo[TO_NUM(iter.first)].userInputState;
		if (keyboardInputState == EUserInputState::Count)
		{
			continue;
		}

		const Callback& keyboardInputCallback = iter.second.arrCallback[TO_NUM(keyboardInputState)];
		if (keyboardInputCallback != nullptr)
		{
			keyboardInputCallback(iter.second.callbackArgs);
		}
	}

	for (auto& refIter : m_mapKeyboardHoldingInputBoundInfo)
	{
		EUserInputState keyboardInputState = m_arrCurrentKeyboardUserInputInfo[TO_NUM(refIter.first)].userInputState;
		if (keyboardInputState == EUserInputState::Count)
		{
			continue;
		}

		// Ȧ���� ���� Ÿ�̸� ����
		Timer& refKeyboardInputHoldingTimer = refIter.second.timer;
		if (keyboardInputState == EUserInputState::Pressing)
		{
			refKeyboardInputHoldingTimer.Update(deltaSeconds);
		}
		else if (keyboardInputState == EUserInputState::Up)
		{
			refKeyboardInputHoldingTimer.SetSwitch(true);
		}
		else
		{
			refKeyboardInputHoldingTimer.Reset();
		}
	}

	return true;
}

void KeyboardInputHandler::BindKeyboardInput(EKeyboardValue keyboardValue,
	const Callback& downCallback, const Callback& pressingCallback, const Callback& upCallback)
{
	if (m_mapKeyboardInputBoundInfo.find(keyboardValue) != m_mapKeyboardInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "�̹� �����ϴ� ���ε� ����!");
		return;
	}

	KeyboardInputBoundInfo keyboardInputBoundInfo;
	keyboardInputBoundInfo.keyboardValue = keyboardValue;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Down)] = downCallback;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Pressing)] = pressingCallback;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EUserInputState::Up)] = upCallback;

	const auto& retInsert = m_mapKeyboardInputBoundInfo.insert(std::make_pair(keyboardValue, keyboardInputBoundInfo));
	ASSERT_LOG(retInsert.second == true);
}

void KeyboardInputHandler::BindKeyboardHoldingInput(EKeyboardValue keyboardValue, float targetTime, const Callback& holdingEndCallback)
{
	if (m_mapKeyboardHoldingInputBoundInfo.find(keyboardValue) != m_mapKeyboardHoldingInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "�̹� �����ϴ� Ȧ�� ���ε� ����!");
		return;
	}

	KeyboardHoldingInputBoundInfo keyboardHoldingInputBoundInfo;
	keyboardHoldingInputBoundInfo.keyboardValue = keyboardValue;
	keyboardHoldingInputBoundInfo.timer = TimerManager::I()->CreateTimer(targetTime, false, holdingEndCallback);

	const auto& retInsert = m_mapKeyboardHoldingInputBoundInfo.insert(std::make_pair(keyboardValue, keyboardHoldingInputBoundInfo));
	ASSERT_LOG(retInsert.second == true);
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
