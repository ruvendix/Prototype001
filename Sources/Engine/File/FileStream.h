// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "FileEnum.h"

class FileStream
{
public:
	FileStream();
	~FileStream();

	bool OpenFileStream(const std::string& strFilePath, EFileStreamMode fileSteamMode);
	void CloseFileStream();

	bool ReopenFileStream(EFileStreamMode fileSteamMode);

	bool WriteTextToFileStream(const std::string& strText);
	bool ReadTextFromFileStream(std::string& outStrText);

	bool IsOpenFileStream() const { return (m_pFileStream != nullptr); }

private:
	std::string m_strFilePath;

	FILE* m_pFileStream = nullptr;
	EFileStreamMode m_fileStreamMode = EFileStreamMode::Count;
};