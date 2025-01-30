// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class InputAction
{
public:
	InputAction(const std::string& strInputActionName);
	~InputAction();

	void AddInputMappingInfo(const InputActionMappingInfo& inputMappingInfo);
	bool ProcessInputAction();

	void ApplyInputActionValue(const InputActionValue& inputActionValue, EInputActionValueType inputActionValueType);

	const InputActionValue* GetInputActionValue() const { return (&m_inputActionValue); }
	void SetInputActionDelegator(const DefaultDelegator& inputActionDelegator) { m_inputActionDelegator = inputActionDelegator; }

private:
	std::string m_strInputActionName;
	InputActionValue m_inputActionValue;
	DefaultDelegator m_inputActionDelegator;
	EInputActionValueType m_inputActionValueType = EInputActionValueType::Boolean;

	std::vector<InputActionMappingInfo> m_vecInputActionMappingInfo;
};