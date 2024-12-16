// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using UserInputCallback = std::function<void()>;

struct UserInputInfo
{
	int32 userInputVirtualCode = 0;
	EUserInputState userInputState = EUserInputState::Count;
};

struct KeyboardInputBoundInfo
{
	EKeyboardValue keyboardValue = EKeyboardValue::Count; // �̰� ������ ��
	std::array<UserInputCallback, TO_NUM(EUserInputState::Count)> arrCallback;
};

struct KeyboardHoldingInputBoundInfo
{
	EKeyboardValue keyboardValue = EKeyboardValue::Count; // �̰� ������ ��
	Timer timer; // Ȧ���� Ÿ�̸� �ʿ�, Ÿ�̸ӿ� �ݹ��� ����
};