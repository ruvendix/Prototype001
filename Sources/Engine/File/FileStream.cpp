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

bool FileStream::WriteSingleLineTextToFileStream(const std::string& strSingleLineText) const
{
	if ((CheckValidFileStreamMode(EFileStreamMode::WriteOnlyText) == false) &&
		(CheckValidFileStreamMode(EFileStreamMode::AppendText) == false))
	{
		return false;
	}

	std::fprintf(m_pFileStream, "%s\n", strSingleLineText.c_str());
	return true;
}

bool FileStream::ReadSingleLineTextToFileStream(std::string& outStrSingleLineText) const
{
	if (CheckValidFileStreamMode(EFileStreamMode::ReadOnlyText) == false)
	{
		return false;
	}

	char stringBuffer[1024] = { '\0' }; // 한줄에 1024 바이트로 제한
	fscanf_s(m_pFileStream, "%[^\n]", stringBuffer, ARRAYSIZE(stringBuffer));
	outStrSingleLineText = stringBuffer;

	return true;
}

bool FileStream::WriteStringToFileStream(const std::string& strContents) const
{
	if (CheckValidFileStreamMode(EFileStreamMode::WriteOnlyBinary) == false)
	{
		return false;
	}

	int32 strLength = static_cast<int32>(strContents.length());
	int32 wroteCount = std::fwrite(&strLength, sizeof(int32), 1, m_pFileStream);
	if (wroteCount != 1)
	{
		return false;
	}

	int32 wroteBytes = std::fwrite(strContents.c_str(), sizeof(char), strLength, m_pFileStream);
	if (wroteBytes != strLength)
	{
		return false;
	}

	return true;
}

bool FileStream::ReadStringFromFileStream(std::string& outStrContents) const
{
	if (CheckValidFileStreamMode(EFileStreamMode::ReadOnlyBinary) == false)
	{
		return false;
	}

	// 스트링은 몇 글자를 썼는지를 알아야함
	int32 strLength = 0;
	int32 readCount = std::fread(&strLength, sizeof(int32), 1, m_pFileStream);
	if (readCount != 1)
	{
		return false;
	}

	char* stringBuffer = static_cast<char*>(std::malloc(sizeof(char) * (strLength + 1))); // 종료 문자 포함
	if (stringBuffer != nullptr)
	{
		int32 readBytes = std::fread(stringBuffer, sizeof(char), strLength, m_pFileStream);
		if (readBytes != strLength)
		{
			return false;
		}

		stringBuffer[strLength] = '\0';
		outStrContents = stringBuffer;
		std::free(stringBuffer);
	}

	return true;
}

bool FileStream::CheckValidFileStreamMode(EFileStreamMode fileStreamMode) const
{
	if ((IsOpenFileStream() == false) ||
		(m_fileStreamMode != fileStreamMode))
	{
		return false;
	}

	return true;
}