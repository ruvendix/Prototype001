// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "TraceLogger.h"

void TraceLogger::Excute(const std::string& strLog)
{
	std::string strLineFeedLog = (strLog + '\n');
	::OutputDebugString(strLineFeedLog.c_str());
}

bool TraceLogger::CheckVerbosity(ELogVerbosity logVerbosity) const
{
	if ((logVerbosity == ELogVerbosity::OnlyTrace) ||
		(logVerbosity == ELogVerbosity::Assert) ||
		(logVerbosity == ELogVerbosity::Notice) ||
		(logVerbosity == ELogVerbosity::Error) ||
		(logVerbosity == ELogVerbosity::Display))
	{
		return true;
	}

	return false;
}