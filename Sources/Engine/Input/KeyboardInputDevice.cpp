// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "KeyboardInputDevice.h"

#include "InputMacros.h"

DEFINE_SINGLETON(KeyboardInputDevice);

void KeyboardInputDevice::Startup()
{
	// �ʱ�ȭ
	InputInfo keyboardInputInfo;
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

bool KeyboardInputDevice::Update(float deltaSeconds)
{
	UNREFERENCED_PARAMETER(deltaSeconds);

	// ������ ���� Ű���� �Է� ���¸� Ȯ��
	int32 keyboardValueCount = TO_NUM(EKeyboardValue::Count);
	for (int32 i = 0; i < keyboardValueCount; ++i)
	{
		InputInfo& refKeyboardInputInfo = m_arrCurrentKeyboardUserInputInfo[i];
		bool bExistKeyboardInput = IS_EXIST_USER_INPUT(refKeyboardInputInfo.userInputVirtualCode);
		
		switch (refKeyboardInputInfo.userInputState)
		{
		case EInputTrigger::Pressed:
		{
			if (bExistKeyboardInput == true)
			{
				refKeyboardInputInfo.userInputState = EInputTrigger::Holding;
			}
			else
			{
				refKeyboardInputInfo.userInputState = EInputTrigger::Released;
			}

			break;
		}

		case EInputTrigger::Holding:
		{
			if (bExistKeyboardInput == false)
			{
				refKeyboardInputInfo.userInputState = EInputTrigger::Released;
			}

			break;
		}

		case EInputTrigger::Released:
		{
			if (bExistKeyboardInput == true)
			{
				refKeyboardInputInfo.userInputState = EInputTrigger::Pressed;
			}
			else
			{
				refKeyboardInputInfo.userInputState = EInputTrigger::Count;
			}

			break;
		}

		// Down���� ��ȯ��
		case EInputTrigger::Count:
		{
			if (bExistKeyboardInput == true)
			{
				refKeyboardInputInfo.userInputState = EInputTrigger::Pressed;
			}

			break;
		}
		}
	}

	for (const auto& iter : m_mapKeyboardInputBoundInfo)
	{
		EInputTrigger keyboardInputState = m_arrCurrentKeyboardUserInputInfo[TO_NUM(iter.first)].userInputState;
		if (keyboardInputState == EInputTrigger::Count)
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
		EInputTrigger keyboardInputState = m_arrCurrentKeyboardUserInputInfo[TO_NUM(refIter.first)].userInputState;
		if (keyboardInputState == EInputTrigger::Count)
		{
			continue;
		}

		// Ȧ���� ���� Ÿ�̸� ����
		Timer& refKeyboardInputHoldingTimer = refIter.second.timer;
		if (keyboardInputState == EInputTrigger::Holding)
		{
			refKeyboardInputHoldingTimer.Update(deltaSeconds);
		}
		else if (keyboardInputState == EInputTrigger::Released)
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

void KeyboardInputDevice::BindKeyboardInput(EKeyboardValue keyboardValue,
	const Callback& downCallback, const Callback& pressingCallback, const Callback& upCallback)
{
	if (m_mapKeyboardInputBoundInfo.find(keyboardValue) != m_mapKeyboardInputBoundInfo.cend())
	{
		TRACE_LOG(LogDefault, "�̹� �����ϴ� ���ε� ����!");
		return;
	}

	KeyboardInputBoundInfo keyboardInputBoundInfo;
	keyboardInputBoundInfo.keyboardValue = keyboardValue;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EInputTrigger::Pressed)] = downCallback;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EInputTrigger::Holding)] = pressingCallback;
	keyboardInputBoundInfo.arrCallback[TO_NUM(EInputTrigger::Released)] = upCallback;

	const auto& retInsert = m_mapKeyboardInputBoundInfo.insert(std::make_pair(keyboardValue, keyboardInputBoundInfo));
	ASSERT_LOG(retInsert.second == true);
}

void KeyboardInputDevice::BindKeyboardHoldingInput(EKeyboardValue keyboardValue, float targetTime, const Callback& holdingEndCallback)
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

bool KeyboardInputDevice::CheckKeyboardValueDown(EKeyboardValue keyboardValue) const
{
	return (m_arrCurrentKeyboardUserInputInfo[TO_NUM(keyboardValue)].userInputState == EInputTrigger::Pressed);
}

bool KeyboardInputDevice::CheckKeyboardValuePressing(EKeyboardValue keyboardValue) const
{
	return (m_arrCurrentKeyboardUserInputInfo[TO_NUM(keyboardValue)].userInputState == EInputTrigger::Holding);
}

bool KeyboardInputDevice::CheckKeyboardValueUp(EKeyboardValue keyboardValue) const
{
	return (m_arrCurrentKeyboardUserInputInfo[TO_NUM(keyboardValue)].userInputState == EInputTrigger::Released);
}

bool KeyboardInputDevice::CheckActivateInputTrigger(const InputMappingInfo& inputMappingInfo) const
{
	// �ϴ��� �������� ����


	return false;
}