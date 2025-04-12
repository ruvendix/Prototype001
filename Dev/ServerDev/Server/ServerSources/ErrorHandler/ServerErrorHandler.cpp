// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ServerErrorHandler.h"

DEFINE_SINGLETON(ServerErrorHandler);

void ServerErrorHandler::Startup()
{
	InsertErrorInfo(EServerErrorCode::EntityInsertFailed, &g_LogDefault, "��Ʈ��ũ ���� ��ü ��� ����!");
}