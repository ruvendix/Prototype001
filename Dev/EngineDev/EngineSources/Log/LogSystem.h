// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "LogEnums.h"

class ILogger;
class LogCategory;

DECLARE_COMPILETIMER_COUNTER(LoggerIdCounter);

class LogSystem : public ICoreLoop
{
	DECLARE_SINGLETON(LogSystem)

public:
	virtual void Startup() override;

public:
	void RegisterFileLoggerPathKey(const std::string& strFileLoggerPathKey);

	void Log(const LogCategory* pLogCategory, const std::string& strLog,
		ELogVerbosity logVerbosity, const char* szFilePath, int32 fileLine, const char* szFunction);

private:
	std::unordered_map<uint32, std::shared_ptr<ILogger>> m_mapLogger;
	EnumBitset<ELogAdditionalOption, TO_NUM(ELogAdditionalOption::Count)> m_bitsetLogAdditionalOption;
};