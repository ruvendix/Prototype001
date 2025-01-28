// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "InputEnums.h"
#include "InputStructs.h"

class KeyboardInputDevice : public ICoreLoop
{
	DECLARE_SINGLETON(KeyboardInputDevice)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	void BindKeyboardInput(EKeyboardValue keyboardValue,
		const Callback& downCallback, const Callback& pressingCallback, const Callback& upCallback);

	void BindKeyboardHoldingInput(EKeyboardValue keyboardValue, float targetTime, const Callback& holdingEndCallback);

	// 유저 입력에 대한 바인딩 정보가 없어도 사용 가능
	bool CheckKeyboardValueDown(EKeyboardValue keyboardValue) const;
	bool CheckKeyboardValuePressing(EKeyboardValue keyboardValue) const;
	bool CheckKeyboardValueUp(EKeyboardValue keyboardValue) const;

	EInputValueState GetCurrentInputValueState(EInputValue inputValue) const { return m_arrCurrentKeyboardUserInputInfo[TO_NUM(inputValue)].inputValueState; }

private:
	std::array<InputRawInfo, TO_NUM(EInputValue::Count)> m_arrCurrentKeyboardUserInputInfo;

	std::unordered_map<EKeyboardValue, KeyboardInputBoundInfo> m_mapKeyboardInputBoundInfo;
	std::unordered_map<EKeyboardValue, KeyboardHoldingInputBoundInfo> m_mapKeyboardHoldingInputBoundInfo;
};