// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "LogSystem.h"

#include "LogCategory.h"
#include "EngineSources/Log/Logger/ILogger.h"
#include "EngineSources/Log/Logger/TraceLogger.h"
#include "EngineSources/Log/Logger/PopUpLogger.h"
#include "EngineSources/Log/Logger/FileLogger.h"

DEFINE_SINGLETON(LogSystem);
DEFINE_COMPILETIMER_COUNTER(LoggerIdCounter);

void LogSystem::Startup()
{
	// 전부 활성화
	m_bitsetLogAdditionalOption.BitsOn(
			ELogAdditionalOption::Time,
			ELogAdditionalOption::FilePath,
			ELogAdditionalOption::FileLine,
			ELogAdditionalOption::Function);

	auto insertedIter = m_mapLogger.insert(std::make_pair(TraceLogger::s_id, std::make_shared<TraceLogger>()));
	if (insertedIter.second == false)
	{
		return;
	}

	insertedIter = m_mapLogger.insert(std::make_pair(PopupLogger::s_id, std::make_shared<PopupLogger>()));
	if (insertedIter.second == false)
	{
		return;
	}

	insertedIter = m_mapLogger.insert(std::make_pair(FileLogger::s_id, std::make_shared<FileLogger>()));
	if (insertedIter.second == false)
	{
		return;
	}
}

void LogSystem::RegisterFileLoggerPathKey(const std::string& strFileLoggerPathKey)
{
	auto foundIter = m_mapLogger.find(FileLogger::s_id);
	if (foundIter == m_mapLogger.cend())
	{
		return;
	}

	const std::shared_ptr<FileLogger>& spFileLogger = std::dynamic_pointer_cast<FileLogger>(foundIter->second);
	if (spFileLogger == nullptr)
	{
		return;
	}

	spFileLogger->RegisterLogPathKey(strFileLoggerPathKey);
}

void LogSystem::Log(const LogCategory* pLogCategory, const std::string& strLog, ELogVerbosity logVerbosity, const char* szFilePath, int32 fileLine, const char* szFunction) const
{
	if (m_mapLogger.empty() == true)
	{
		return;
	}

	// 로그 카테고리를 꺼뒀다면 해당 로그는 무시합니다.
	if (pLogCategory->IsOn() == false)
	{
		return;
	}

	std::string strFullLog; // [로그 카테고리]: [로그 내용] [현재 시간] [로그가 찍힌 파일(줄 번호)] [로그가 찍힌 함수]
	strFullLog = '[' + (pLogCategory->GetCategoryName()) + "]: ";
	strFullLog += strLog;

	if (m_bitsetLogAdditionalOption.IsBitOn(ELogAdditionalOption::Time))
	{
		strFullLog += ' ';

		// 이건 진짜 시간이 있어야하네 패스
		std::string strLocalTime;
		strLocalTime += '[';
		RenewLocalTimeString(strLocalTime);
		strLocalTime += ']';
		strFullLog += strLocalTime;
	}

	if (m_bitsetLogAdditionalOption.IsBitOn(ELogAdditionalOption::FilePath))
	{
		strFullLog += " [";
		strFullLog += szFilePath;

		if (m_bitsetLogAdditionalOption.IsBitOn(ELogAdditionalOption::FileLine))
		{
			strFullLog += ('(' + std::to_string(fileLine) + ')');
		}

		strFullLog += ']';
	}

	if (m_bitsetLogAdditionalOption.IsBitOn(ELogAdditionalOption::Function))
	{
		strFullLog += ' ';

		std::string strFunction;
		strFunction += "[Function: ";
		strFunction += szFunction;
		strFunction += ']';
		strFullLog += strFunction;
	}

	for (const auto& iter : m_mapLogger)
	{
		const std::shared_ptr<ILogger>& spLogger = iter.second;
		if ((spLogger != nullptr) &&
			(spLogger->CheckVerbosity(logVerbosity) == true))
		{
			spLogger->Excute(strFullLog);
		}
	}

#if _DEBUG
	if ((logVerbosity == ELogVerbosity::Error) ||
		(logVerbosity == ELogVerbosity::Assert))
	{
		::__debugbreak(); // 윈도우 전용
	}
#endif
}