// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientErrorHandler.h"

DEFINE_SINGLETON(ClientErrorHandler);

void ClientErrorHandler::Startup()
{
	InsertErrorInfo(EClientErrorCode::ConnectFailedToServer, &g_LogDefault, "서버로 연결 실패!");
	InsertErrorInfo(EClientErrorCode::DisconnectFromServer, &g_LogDefault, "서버로부터 연결 끊김!");
}