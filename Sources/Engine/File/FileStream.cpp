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
	// 파일 스트림 닫고 시작
	CloseFileStream();

	// 파일 스트림 열기
	m_fileStream.open(strFilePath, std::ios::in | std::ios::out | std::ios::app); // app은 append 약자

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

	// 파일 패스가 있다면 검증도 해야함

	return (OpenFileStream(m_strFilePath));
}

bool FileStream::WriteToFileStream(const std::string& strContents)
{
	if (IsOpenFileStream())
	{
		m_fileStream << strContents << std::endl;
		return true;
	}

	return false; // 파일이 열려있지 않은 경우
}

bool FileStream::ReadFromFileStream(std::string& outStrContents)
{
	if (IsOpenFileStream())
	{
		std::getline(m_fileStream, outStrContents);
		return true;
	}

	return false; // 파일이 열려있지 않은 경우
}