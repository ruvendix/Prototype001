// Copyright 2022 Ruvendix, All Rights Reserved.

#include "PCH.h"
#include "FileLogger.h"

#include "EngineSources/File/PathManager.h"

DEFINE_COMPILETIME_ID(FileLogger, LoggerIdCounter)

FileLogger::FileLogger()
{
	
}

FileLogger::~FileLogger()
{

}

void FileLogger::Excute(const std::string& strLog)
{
	{ // 크리티컬 섹션
		std::lock_guard lock(m_logFileMutex);
		if (m_logFileStream.ReopenFileStream(EFileStreamMode::AppendText) == false)
		{
			std::string strLocalTime;
			RenewLocalTimeString(strLocalTime);

			std::string strLogFilePath = PathManager::I()->FindPath(m_strLogPathKey).string() + strLocalTime;
			strLogFilePath += ".log";
			m_logFileStream.OpenFileStream(strLogFilePath, EFileStreamMode::WriteOnlyText);
		}

		m_logFileStream.WriteSingleLineTextToFileStream(strLog);
	}

	m_logFileStream.CloseFileStream();
}

bool FileLogger::CheckVerbosity(ELogVerbosity logVerbosity) const
{
	if ((logVerbosity == ELogVerbosity::OnlyFile) ||
		(logVerbosity == ELogVerbosity::Notice) ||
		(logVerbosity == ELogVerbosity::Error) ||
		(logVerbosity == ELogVerbosity::Display))
	{
		return true;
	}

	return false;
}

void FileLogger::RegisterLogPathKey(const std::string& strLogPathKey)
{
	m_strLogPathKey = strLogPathKey;
	PathManager::I()->AddPathAtCurrentWorkingDirectory(strLogPathKey);
}