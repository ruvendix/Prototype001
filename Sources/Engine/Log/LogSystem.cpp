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
	// ���� Ȱ��ȭ
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
		::__debugbreak(); // ������ ����
	}
#endif
}