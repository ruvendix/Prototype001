// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "UserInputEnums.h"
#include "UserInputStruct.h"

class KeyboardInputHandler : public ICoreLoop
{
	DECLARE_SINGLETON(KeyboardInputHandler)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	void BindKeyboardInput(EKeyboardValue keyboardValue,
		const Callback& downCallback, const Callback& pressingCallback, const Callback& upCallback);

	void BindKeyboardHoldingInput(EKeyboardValue keyboardValue, float targetTime, const Callback& holdingEndCallback);

	// ���� �Է¿� ���� ���ε� ������ ��� ��� ����
	bool CheckKeyboardValueDown(EKeyboardValue keyboardValue) const;
	bool CheckKeyboardValuePressing(EKeyboardValue keyboardValue) const;
	bool CheckKeyboardValueUp(EKeyboardValue keyboardValue) const;

private:
	std::array<UserInputInfo, TO_NUM(EKeyboardValue::Count)> m_arrCurrentKeyboardUserInputInfo;

	std::unordered_map<EKeyboardValue, KeyboardInputBoundInfo> m_mapKeyboardInputBoundInfo;
	std::unordered_map<EKeyboardValue, KeyboardHoldingInputBoundInfo> m_mapKeyboardHoldingInputBoundInfo;
};