// Copyright 2022 Ruvendix, All Rights Reserved.

#pragma once

class TraceLogger : public ILogger
{
public:
	TraceLogger() = default;
	virtual ~TraceLogger() = default;

	virtual void Excute(const std::string& strLog) override;
	virtual bool CheckVerbosity(ELogVerbosity logVerbosity) const override;
};