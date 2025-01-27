// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class InputAction
{
public:
	InputAction(const std::string& strInputActionName);
	~InputAction();

	void ModifyInputActionValue(bool bValue);

	void AddInputMappingInfo(const InputMappingInfo& inputMappingInfo);
	bool ProcessInputAction();

	void SetInputActionValue(const InputActionValue& inputActionValue) { m_inputActionValue = inputActionValue; }
	const InputActionValue* GetInputActionValue() const { return (&m_inputActionValue); }

	void SetInputActionHandler(const InputActionHandler& inputActionHandler) { m_inputActionHandler = inputActionHandler; }

private:
	std::string m_strInputActionName;
	InputActionValue m_inputActionValue;
	InputActionHandler m_inputActionHandler = nullptr;
	std::vector<InputMappingInfo> m_vecInputMappingInfo;
};