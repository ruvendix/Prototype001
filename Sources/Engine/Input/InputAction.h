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
	void SetInputActionHandler(const InputActionHandler& inputActionHandler) { m_inputActionHandler = inputActionHandler; }

private:
	std::string m_strInputActionName;
	InputActionValue m_inputActionValue;
	InputActionHandler m_inputActionHandler = nullptr;
	EInputActionValueType m_inputActionValueType = EInputActionValueType::Boolean;

	std::vector<InputActionMappingInfo> m_vecInputMappingInfo;
};