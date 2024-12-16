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
	EKeyboardValue keyboardValue = EKeyboardValue::Count; // 이건 유일한 값
	std::array<UserInputCallback, TO_NUM(EUserInputState::Count)> arrCallback;
};

struct KeyboardHoldingInputBoundInfo
{
	EKeyboardValue keyboardValue = EKeyboardValue::Count; // 이건 유일한 값
	Timer timer; // 홀딩은 타이머 필요, 타이머에 콜백이 있음
};