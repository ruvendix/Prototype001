// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ErrorHandler.h"

DEFINE_SINGLETON(ErrorHandler);

void ErrorHandler::Startup()
{
	InsertErrorInfo(EErrorCode::Unknown, &g_LogDefault, "알 수 없는 오류!");
	InsertErrorInfo(EErrorCode::ExistedResource, &g_LogResource, "이미 로딩된 리소스!");
	InsertErrorInfo(EErrorCode::ExistedComponent, &g_LogResource, "이미 갖고 있는 컴포넌트!");
	InsertErrorInfo(EErrorCode::NotFoundResource, &g_LogResource, "등록 또는 로딩이 되지 않은 리소스!");
	InsertErrorInfo(EErrorCode::InvalidIntegerRange, &g_LogDefault, "유효하지 않은 정수 범위!");
}

void ErrorHandler::InsertErrorInfo(EErrorCode errorCode, const LogCategory* pLogCategory, const std::string& strFormatError)
{
	ErrorInfo errorInfo{ pLogCategory, strFormatError };

	const auto& retter = m_mapErrorInfo.insert(std::make_pair(errorCode, errorInfo));
	if (retter.second == false)
	{
		ERROR_LOG(LogDefault, "오류 내용 등록 실패!");
	}
}

const ErrorInfo* ErrorHandler::FindErrorInfo(EErrorCode errorCode) const
{
	const auto& foundIter = m_mapErrorInfo.find(errorCode);
	if (foundIter == m_mapErrorInfo.cend())
	{
		return nullptr;
	}

	return &(foundIter->second);
}