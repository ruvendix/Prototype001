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
	// ���� Ȱ��ȭ
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

	// �α� ī�װ��� ���״ٸ� �ش� �α״� �����մϴ�.
	if (pLogCategory->IsOn() == false)
	{
		return;
	}

	std::string strFullLog; // [�α� ī�װ�]: [�α� ����] [���� �ð�] [�αװ� ���� ����(�� ��ȣ)] [�αװ� ���� �Լ�]
	strFullLog = '[' + (pLogCategory->GetCategoryName()) + "]: ";
	strFullLog += strLog;

	if (m_bitsetLogAdditionalOption.IsBitOn(ELogAdditionalOption::Time))
	{
		strFullLog += ' ';

		// �̰� ��¥ �ð��� �־���ϳ� �н�
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
		::__debugbreak(); // ������ ����
	}
#endif
}