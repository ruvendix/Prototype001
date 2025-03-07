// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ErrorHandler.h"

DEFINE_SINGLETON(ErrorHandler);

void ErrorHandler::Startup()
{
	InsertErrorInfo(EErrorCode::Unknown, &g_LogDefault, "�� �� ���� ����!");
	InsertErrorInfo(EErrorCode::ExistedResource, &g_LogResource, "�̹� �ε��� ���ҽ�!");
	InsertErrorInfo(EErrorCode::ExistedComponent, &g_LogResource, "�̹� ���� �ִ� ������Ʈ!");
	InsertErrorInfo(EErrorCode::NotFoundResource, &g_LogResource, "��� �Ǵ� �ε��� ���� ���� ���ҽ�!");
	InsertErrorInfo(EErrorCode::InvalidIntegerRange, &g_LogDefault, "��ȿ���� ���� ���� ����!");
	InsertErrorInfo(EErrorCode::InvalidRenderingLayer, &g_LogDefault, "��ȿ���� ���� ������ ���̾�!");
	InsertErrorInfo(EErrorCode::InvalidFileStream, &g_LogResource, "��ȿ���� ���� ���� ��Ʈ��!");	
	InsertErrorInfo(EErrorCode::NoReturnNullObject, &g_LogDefault, "�� ��ü�� ��ȯ�� ������ �̹� ���� ��Ȳ!");
	InsertErrorInfo(EErrorCode::ServerGameEntityInsertFailed, &g_LogDefault, "�������� ���� ��ü ��� ����!");	
}

void ErrorHandler::InsertErrorInfo(EErrorCode errorCode, const LogCategory* pLogCategory, const std::string& strFormatError)
{
	ErrorInfo errorInfo{ pLogCategory, strFormatError };

	auto insertedIter = m_mapErrorInfo.insert(std::make_pair(errorCode, errorInfo));
	if (insertedIter.second == false)
	{
		ERROR_LOG(LogDefault, "���� ���� ��� ����!");
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