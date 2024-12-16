// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "FileStream.h"

FileStream::FileStream()
{

}

FileStream::~FileStream()
{
	CloseFileStream();
}

bool FileStream::OpenFileStream(const std::string& strFilePath)
{
	// ���� ��Ʈ�� �ݰ� ����
	CloseFileStream();

	// ���� ��Ʈ�� ����
	m_fileStream.open(strFilePath, std::ios::in | std::ios::out | std::ios::app); // app�� append ����

	if (IsOpenFileStream())
	{
		m_strFilePath = strFilePath;
		return true;
	}

	return false;
}

void FileStream::CloseFileStream()
{
	if (IsOpenFileStream())
	{
		m_fileStream.close();
	}
}

bool FileStream::ReopenFileStream()
{
	if (m_strFilePath.empty() == true)
	{
		return false;
	}

	// ���� �н��� �ִٸ� ������ �ؾ���

	return (OpenFileStream(m_strFilePath));
}

bool FileStream::WriteToFileStream(const std::string& strContents)
{
	if (IsOpenFileStream())
	{
		m_fileStream << strContents << std::endl;
		return true;
	}

	return false; // ������ �������� ���� ���
}

bool FileStream::ReadFromFileStream(std::string& outStrContents)
{
	if (IsOpenFileStream())
	{
		std::getline(m_fileStream, outStrContents);
		return true;
	}

	return false; // ������ �������� ���� ���
}