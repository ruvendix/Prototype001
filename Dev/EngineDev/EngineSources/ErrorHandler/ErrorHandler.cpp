// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ErrorHandler.h"

DEFINE_SINGLETON(EngineErrorHandler);

void EngineErrorHandler::Startup()
{
	InsertErrorInfo(EEngineErrorCode::Unknown, &g_LogDefault, "알 수 없는 오류!");
	InsertErrorInfo(EEngineErrorCode::ExistedResource, &g_LogResource, "이미 로딩된 리소스!");
	InsertErrorInfo(EEngineErrorCode::NotFoundResource, &g_LogResource, "등록 또는 로딩이 되지 않은 리소스!");
	InsertErrorInfo(EEngineErrorCode::InvalidIntegerRange, &g_LogDefault, "유효하지 않은 정수 범위!");
	InsertErrorInfo(EEngineErrorCode::InvalidRenderingLayer, &g_LogDefault, "유효하지 않은 렌더링 레이어!");
	InsertErrorInfo(EEngineErrorCode::InvalidFileStream, &g_LogResource, "유효하지 않은 파일 스트림!");
	InsertErrorInfo(EEngineErrorCode::NoReturnNullObject, &g_LogDefault, "널 객체를 반환할 정도면 이미 망한 상황!");
}