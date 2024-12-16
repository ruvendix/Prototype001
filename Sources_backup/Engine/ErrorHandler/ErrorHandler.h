// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "ErrorEnum.h"

struct ErrorInfo
{
	const LogCategory* pLogCategory = nullptr;
	std::string strFormatError;
};

class ErrorHandler : public IGameLoop
{
	DECLARE_SINGLETON(ErrorHandler)

public:
	virtual void Startup() override;

	void InsertErrorInfo(EErrorCode errorCode, const LogCategory* pLogCategory, const std::string& strFormatError);
	const ErrorInfo* FindErrorInfo(EErrorCode errorCode) const;

	template <typename... Args>
	void ProcessErrorContents(EErrorCode errorCode, Args&&... args)
	{
		const ErrorInfo* pFoundErrorInfo = FindErrorInfo(errorCode);
		if (pFoundErrorInfo == nullptr)
		{
			return;
		}

		std::string strErrorContents;
		MakeFormatString(strErrorContents, pFoundErrorInfo->strFormatError.c_str(), std::forward<Args>(args)...);
		LogSystem::I()->Log(pFoundErrorInfo->pLogCategory, strErrorContents, ELogVerbosity::Error, __FILE__, __LINE__, __FUNCTION__);
	}

private:
	std::unordered_map<EErrorCode, ErrorInfo> m_mapErrorInfo;
};