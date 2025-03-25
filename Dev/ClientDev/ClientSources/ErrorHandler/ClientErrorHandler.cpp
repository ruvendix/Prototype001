// Copyright 2025 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ClientErrorHandler.h"

DEFINE_SINGLETON(ClientErrorHandler);

void ClientErrorHandler::Startup()
{
	InsertErrorInfo(EClientErrorCode::ConnectFailedToServer, &g_LogDefault, "������ ���� ����!");
	InsertErrorInfo(EClientErrorCode::DisconnectFromServer, &g_LogDefault, "�����κ��� ���� ����!");
}