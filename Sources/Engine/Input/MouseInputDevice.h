// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "InputEnums.h"
#include "InputStructs.h"

class MouseInputDevice : public ICoreLoop
{
	DECLARE_SINGLETON(MouseInputDevice)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	void BindMouseInput(EMouseValue mouseValue, const Callback& downCallback, const Callback& pressingCallback, const Callback& upCallback);
	void UnbindMouseInput(EMouseValue mouseValue);

	void BindMouseHoldingInput(EMouseValue mouseValue, float targetTime, const Callback& holdingEndCallback);
	void UnbindMouseHoldingInput(EMouseValue mouseValue);

	// ���� �Է¿� ���� ���ε� ������ ��� ��� ����
	bool CheckMouseValueDown(EMouseValue mouseValue) const;
	bool CheckMouseValuePressing(EMouseValue mouseValue) const;
	bool CheckMouseValueUp(EMouseValue mouseValue) const;

	const Position2d& GetCurrentMousePosition() const { return m_currentMousePos; }

private:
	std::array<InputRawInfo, TO_NUM(EMouseValue::Count)> m_arrCurrentMouseUserInputInfo;

	std::unordered_map<EMouseValue, MouseInputBoundInfo> m_mapMouseInputBoundInfo;
	std::unordered_map<EMouseValue, MouseHoldingInputBoundInfo> m_mapMouseHoldingInputBoundInfo;

	// ���콺 ��ǥ ���� ����
	Position2d m_currentMousePos;
};