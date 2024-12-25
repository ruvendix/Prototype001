// Copyright 2022 Ruvendix, All Rights Reserved.

#include "PCH.h"
#include "FileLogger.h"

#include "Engine/File/PathManager.h"

namespace
{
	const char* g_szLogPathKey = "Log";
}

FileLogger::FileLogger()
{
	// 로그 파일 경로를 미리 생성
	PathManager::I()->AddPathAtCurrentWorkingDirectory(g_szLogPathKey);
}

FileLogger::~FileLogger()
{

}

void FileLogger::Excute(const std::string& strLog)
{
	{ // 크리티컬 섹션
		std::lock_guard lock(m_logFileMutex);
		if (m_logFileStream.ReopenFileStream() == false)
		{
			std::string strLocalTime;
			RenewLocalTimeString(strLocalTime);

			std::string strLogFilePath = PathManager::I()->FindPath(g_szLogPathKey).string() + strLocalTime;
			strLogFilePath += ".log";
			m_logFileStream.OpenFileStream(strLogFilePath); // 여기는 PathManager 달아야하고
		}

		m_logFileStream.WriteToFileStream(strLog);
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