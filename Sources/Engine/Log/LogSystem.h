// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "LogEnum.h"

class ILogger;
class LogCategory;

class LogSystem : public IGameLoop
{
	DECLARE_SINGLETON(LogSystem)

public:
	virtual void Startup() override;

	void Log(const LogCategory* pLogCategory, const std::string& strLog,
		ELogVerbosity logVerbosity, const char* szFilePath, int32 fileLine, const char* szFunction);

private:
	std::vector<std::unique_ptr<ILogger>> m_vecLogger;
	EnumBitset<ELogAdditionalOption, TO_NUM(ELogAdditionalOption::Count)> m_bitsetLogAdditionalOption;
};