// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PopUpLogger.h"

void PopupLogger::Excute(const std::string& strLog)
{
	::MessageBox(nullptr, strLog.c_str(), nullptr, MB_ICONERROR);
}

bool PopupLogger::CheckVerbosity(ELogVerbosity logVerbosity) const
{
	if ((logVerbosity == ELogVerbosity::OnlyPopup) ||
		(logVerbosity == ELogVerbosity::Assert) ||
		(logVerbosity == ELogVerbosity::Error))
	{
		return true;
	}

	return false;
}