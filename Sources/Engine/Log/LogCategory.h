// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "LogMacro.h"

class LogCategory
{
public:
	explicit LogCategory(const std::string& strCategoryName);
	~LogCategory() = default;

	void SetOn(bool bOn) { m_bOn = bOn; }
	bool IsOn() const { return (m_bOn == true); }

	const std::string& GetCategoryName() const { return m_strCategoryName; }

private:
	bool m_bOn = true;
	std::string m_strCategoryName;
};