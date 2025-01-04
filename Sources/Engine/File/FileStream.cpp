// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "FileStream.h"

namespace
{
	std::array<const char*, TO_NUM(EFileStreamMode::Count)> g_arrFileStreamModeLookupTable =
	{
		"w", "r", "a", "wb", "rb"
	};
}

FileStream::FileStream()
{

}

FileStream::~FileStream()
{
	CloseFileStream();
}

bool FileStream::OpenFileStream(const std::string& strFilePath, EFileStreamMode fileSteamMode)
{
	// 파일 스트림 닫고 시작
	CloseFileStream();

	// 파일 스트림 모드 알아내기
	const char* szFileStreamMode = g_arrFileStreamModeLookupTable[TO_NUM(fileSteamMode)];

	// 파일 스트림 열기
	fopen_s(&m_pFileStream, strFilePath.c_str(), szFileStreamMode);
	if (IsOpenFileStream() == false)
	{
		return false;
	}

	m_strFilePath = strFilePath;
	m_fileStreamMode = fileSteamMode;

	return true;
}

void FileStream::CloseFileStream()
{
	if (IsOpenFileStream())
	{
		std::fclose(m_pFileStream);
	}
}

bool FileStream::ReopenFileStream(EFileStreamMode fileSteamMode)
{
	if (m_strFilePath.empty() == true)
	{
		return false;
	}

	return (OpenFileStream(m_strFilePath, fileSteamMode));
}

bool FileStream::WriteTextToFileStream(const std::string& strText)
{
	if ((IsOpenFileStream()) &&
		(m_fileStreamMode == EFileStreamMode::WriteOnlyText) ||
		(m_fileStreamMode == EFileStreamMode::AppendText))
	{
		std::fprintf(m_pFileStream, "%s\n", strText.c_str());
		return true;
	}

	return false;
}

bool FileStream::ReadTextFromFileStream(std::string& outStrText)
{
	if ((IsOpenFileStream() == false) ||
		(m_fileStreamMode != EFileStreamMode::ReadOnlyText))
	{
		return false;
	}

	char buffer[1024] = { '\0' }; // 한줄에 1024 바이트로 제한
	fscanf_s(m_pFileStream, "%[^\n]", buffer, ARRAYSIZE(buffer));
	outStrText = buffer;

	return true;
}