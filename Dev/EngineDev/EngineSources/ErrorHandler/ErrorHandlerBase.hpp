// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "ErrorEnums.h"

struct ErrorInfo
{
	const LogCategory* pLogCategory = nullptr;
	std::string strFormatError;
};

template <typename TErrorCodeType>
class ErrorHandlerBase : public ICoreLoop
{
public:
	void InsertErrorInfo(TErrorCodeType errorCode, const LogCategory* pLogCategory, const std::string& strFormatError)
	{
		ErrorInfo errorInfo{ pLogCategory, strFormatError };

		auto insertedIter = m_mapErrorInfo.insert(std::make_pair(errorCode, errorInfo));
		if (insertedIter.second == false)
		{
			ERROR_LOG(LogDefault, "오류 내용 등록 실패!");
		}
	}

	const ErrorInfo* FindErrorInfo(TErrorCodeType errorCode) const
	{
		auto foundIter = m_mapErrorInfo.find(errorCode);
		if (foundIter == m_mapErrorInfo.cend())
		{
			return nullptr;
		}

		return &(foundIter->second);
	}

	template <typename... Args>
	void ProcessErrorContents(TErrorCodeType errorCode, Args&&... args)
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
	std::unordered_map<TErrorCodeType, ErrorInfo> m_mapErrorInfo;
};