// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PopupLogger : public ILogger
{
public:
	PopupLogger() = default;
	virtual ~PopupLogger() = default;

	virtual void Excute(const std::string& strLog) override;
	virtual bool CheckVerbosity(ELogVerbosity logVerbosity) const override;
};