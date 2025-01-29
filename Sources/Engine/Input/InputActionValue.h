// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "InputEnums.h"
#include "InputStructs.h"

DECLARE_LOG_CATEGORY(LogInput);

// 입력 핸들러가 호출되면 정해진 값을 전달하는 역할
class InputActionValue
{
public:
	template <typename TData>
	bool CheckValidDataType() const
	{
		if ((std::is_same_v<TData, bool> == false) &&
			(std::is_same_v<TData, float> == false) &&
			(std::is_same_v<TData, Vec2d> == false))
		{
			ERROR_LOG(LogInput, "유효하지 않은 형식의 값!");
			return false;
		}

		return true;
	}

	template <typename TData>
	void ApplyValue(TData value)
	{
		if (CheckValidDataType<TData>() == false)
		{
			return;
		}

		m_value = value;
	}

	template <typename TData>
	TData BringValue() const
	{
		if (CheckValidDataType<TData>() == false)
		{
			return TData();
		}

		return (std::any_cast<TData>(m_value));
	}

public:
	void ProcessInputActionValue(const InputActionMappingInfo& inputActionMappingInfo, EInputActionValueType inputActionValueType);

private:
	std::any m_value = false;
};