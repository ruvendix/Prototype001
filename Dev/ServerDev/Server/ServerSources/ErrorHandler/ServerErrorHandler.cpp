// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerErrorHandler.h"

DEFINE_SINGLETON(ServerErrorHandler);

void ServerErrorHandler::Startup()
{
	InsertErrorInfo(EServerErrorCode::NetworkGameEntityInsertFailed, &g_LogDefault, "네트워크 게임 개체 등록 실패!");
}