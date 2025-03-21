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
	InsertErrorInfo(EErrorCode::InvalidRenderingLayer, &g_LogDefault, "유효하지 않은 렌더링 레이어!");
	InsertErrorInfo(EErrorCode::InvalidFileStream, &g_LogResource, "유효하지 않은 파일 스트림!");	
	InsertErrorInfo(EErrorCode::NoReturnNullObject, &g_LogDefault, "널 객체를 반환할 정도면 이미 망한 상황!");

	// 이건 클라로 이동
	InsertErrorInfo(EErrorCode::ConnectFailedToServer, &g_LogDefault, "서버로 연결 실패!");
	InsertErrorInfo(EErrorCode::DisconnectFromServer, &g_LogDefault, "서버로부터 연결 끊김!");

	// 이건 서버로 이동
	InsertErrorInfo(EErrorCode::ServerGameEntityInsertFailed, &g_LogDefault, "서버에서 게임 개체 등록 실패!");
}

void ErrorHandler::InsertErrorInfo(EErrorCode errorCode, const LogCategory* pLogCategory, const std::string& strFormatError)
{
	ErrorInfo errorInfo{ pLogCategory, strFormatError };

	auto insertedIter = m_mapErrorInfo.insert(std::make_pair(errorCode, errorInfo));
	if (insertedIter.second == false)
	{
		ERROR_LOG(LogDefault, "오류 내용 등록 실패!");
	}
}

const ErrorInfo* ErrorHandler::FindErrorInfo(EErrorCode errorCode) const
{
	auto foundIter = m_mapErrorInfo.find(errorCode);
	if (foundIter == m_mapErrorInfo.cend())
	{
		return nullptr;
	}

	return &(foundIter->second);
}