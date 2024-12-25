// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/File/FileStream.h"

class FileLogger : public ILogger
{
public:
	FileLogger();
	virtual ~FileLogger();

	virtual void Excute(const std::string& strLog) override;
	virtual bool CheckVerbosity(ELogVerbosity logVerbosity) const override;

private:
	std::mutex m_logFileMutex;
	FileStream m_logFileStream;
};