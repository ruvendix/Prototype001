// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "UserInputEnum.h"
#include "UserInputStruct.h"

class KeyboardInputHandler : public IGameLoop
{
	DECLARE_SINGLETON(KeyboardInputHandler)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	void BindKeyboardInput(EKeyboardValue keyboardValue,
		const UserInputCallback& downCallback, const UserInputCallback& pressingCallback, const UserInputCallback& upCallback);

	void BindKeyboardHoldingInput(EKeyboardValue keyboardValue, float targetTime, const UserInputCallback& holdingEndCallback);

	// 유저 입력에 대한 바인딩 정보가 없어도 사용 가능
	bool CheckKeyboardValueDown(EKeyboardValue keyboardValue) const;
	bool CheckKeyboardValuePressing(EKeyboardValue keyboardValue) const;
	bool CheckKeyboardValueUp(EKeyboardValue keyboardValue) const;

private:
	std::array<UserInputInfo, TO_NUM(EKeyboardValue::Count)> m_arrCurrentKeyboardUserInputInfo;

	std::unordered_map<EKeyboardValue, KeyboardInputBoundInfo> m_mapKeyboardInputBoundInfo;
	std::unordered_map<EKeyboardValue, KeyboardHoldingInputBoundInfo> m_mapKeyboardHoldingInputBoundInfo;
};