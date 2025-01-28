// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "LogSystem.h"

#include "LogCategory.h"
#include "Engine/Log/Logger/ILogger.h"
#include "Engine/Log/Logger/TraceLogger.h"
#include "Engine/Log/Logger/PopUpLogger.h"
#include "Engine/Log/Logger/FileLogger.h"

DEFINE_SINGLETON(LogSystem);

void LogSystem::Startup()
{
	// 전부 활성화
	m_bitsetLogAdditionalOption.BitsOn(
			ELogAdditionalOption::Time,
			ELogAdditionalOption::FilePath,
			ELogAdditionalOption::FileLine,
			ELogAdditionalOption::Function);

	m_vecLogger.push_back(std::make_unique<TraceLogger>());
	m_vecLogger.push_back(std::make_unique<PopupLogger>());
	m_vecLogger.push_back(std::make_unique<FileLogger>());
}

void LogSystem::Log(const LogCategory* pLogCategory, const std::string& strLog,
	ELogVerbosity logVerbosity, const char* szFilePath, int32 fileLine, const char* szFunction)
{
	if (m_vecLogger.empty() == true)
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

	for (const auto& spLogger : m_vecLogger)
	{
		if (spLogger->CheckVerbosity(logVerbosity) == true)
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