// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/File/FileStream.h"

class FileLogger : public ILogger
{
	DECLARE_COMPILETIME_ID;

public:
	FileLogger();
	virtual ~FileLogger();

	virtual void Excute(const std::string& strLog) override;
	virtual bool CheckVerbosity(ELogVerbosity logVerbosity) const override;

public:
	void RegisterLogPathKey(const std::string& strLogPathKey);

private:
	std::string m_strLogPathKey;
	std::mutex m_logFileMutex;
	FileStream m_logFileStream;
};