// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "KeyboardInputHandler.h"

DEFINE_SINGLETON(KeyboardInputHandler);

/*
���� �����ӿ����� ������ �ʾ����� ���� �����ӿ����� ���� ����
���� �����ӿ��� �����µ� ���� �����ӿ����� ���� ����� ��
�ֻ��� ��Ʈ�� 1�� �ǹǷ� 0x8000���� Ȯ�� ���� */
#define IS_EXIST_USER_INPUT(virtualKeyCode) ((::GetAsyncKeyState(virtualKeyCode) & 0x8000) != 0)

void KeyboardInputHandler::Startup()
{
	// �ʱ�ȭ
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

	// ������ ���� Ű���� �Է� ���¸� Ȯ��
	int32 keyboardValueCount = TO_NUM(EKeyboardValue::Count);
	for (int32 i = 0; i < keyboardValueCount; ++i)
	{
		UserInputInfo& refUserInputInfo = m_arrCurrentKeyboardUserInputInfo[i];
		bool bExistUserInput = IS_EXIST_USER_INPUT(refUserInputInfo.userInputVirtualCode);
		
		// �ϴ� �غ��� ���� �������� ��ȯ
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

		// Down���� ��ȯ��
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

		// Ȧ���� ���� Ÿ�̸� ����
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
		TRACE_LOG(LogDefault, "�̹� �����ϴ� ���ε� ����!");
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
		TRACE_LOG(LogDefault, "�̹� �����ϴ� Ȧ�� ���ε� ����!");
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
