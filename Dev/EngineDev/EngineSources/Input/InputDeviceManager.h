// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

class InputDeviceMananger : public ICoreLoop
{
	DECLARE_SINGLETON(InputDeviceMananger)

public:
	virtual void Startup() override;
	virtual bool Update(float deltaSeconds) override;

	virtual EInputValueState GetCurrentInputValueState(EInputValue inputValue) const
	{
		return (m_arrInputRawInfo[TO_NUM(inputValue)].inputValueState);
	}

	const KeyboardDevicePtr& GetKeyboardDevice() const { return m_spKeyboardDevice; }
	const MouseDevicePtr& GetMouseDevice() const { return m_spMouseDevice; }

private:
	std::array<InputRawInfo, TO_NUM(EInputValue::Count)> m_arrInputRawInfo;

	KeyboardDevicePtr m_spKeyboardDevice = nullptr;
	MouseDevicePtr m_spMouseDevice = nullptr;
};