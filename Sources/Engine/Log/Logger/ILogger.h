// Copyright 2022 Ruvendix, All Rights Reserved.

#pragma once

#include "Engine/Log/LogEnum.h"

class ILogger
{
public:
	ILogger() = default;
	virtual ~ILogger() = default;

	virtual void Excute(const std::string& strLog) = 0;
	virtual bool CheckVerbosity(ELogVerbosity logVerbosity) const = 0;
};