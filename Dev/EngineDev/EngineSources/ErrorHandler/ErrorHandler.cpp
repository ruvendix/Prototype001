// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ErrorHandler.h"

DEFINE_SINGLETON(EngineErrorHandler);

void EngineErrorHandler::Startup()
{
	InsertErrorInfo(EEngineErrorCode::Unknown, &g_LogDefault, "�� �� ���� ����!");
	InsertErrorInfo(EEngineErrorCode::ExistedResource, &g_LogResource, "�̹� �ε��� ���ҽ�!");
	InsertErrorInfo(EEngineErrorCode::NotFoundResource, &g_LogResource, "��� �Ǵ� �ε��� ���� ���� ���ҽ�!");
	InsertErrorInfo(EEngineErrorCode::InvalidIntegerRange, &g_LogDefault, "��ȿ���� ���� ���� ����!");
	InsertErrorInfo(EEngineErrorCode::InvalidRenderingLayer, &g_LogDefault, "��ȿ���� ���� ������ ���̾�!");
	InsertErrorInfo(EEngineErrorCode::InvalidFileStream, &g_LogResource, "��ȿ���� ���� ���� ��Ʈ��!");
	InsertErrorInfo(EEngineErrorCode::NoReturnNullObject, &g_LogDefault, "�� ��ü�� ��ȯ�� ������ �̹� ���� ��Ȳ!");
}