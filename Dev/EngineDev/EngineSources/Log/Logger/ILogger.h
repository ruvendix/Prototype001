// Copyright 2022 Ruvendix, All Rights Reserved.

#pragma once

#include "Log/LogEnums.h"

class ILogger
{
	DEFINE_ROOT_COMPILETIME_ID_CLASS;

public:
	ILogger() = default;
	virtual ~ILogger() = default;

	virtual void Excute(const std::string& strLog) = 0;
	virtual bool CheckVerbosity(ELogVerbosity logVerbosity) const = 0;
};