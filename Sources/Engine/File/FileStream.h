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

	bool WriteSingleLineTextToFileStream(const std::string& strSingleLineText) const;
	bool ReadSingleLineTextToFileStream(std::string& outStrSingleLineText) const;

	bool WriteStringToFileStream(const std::string& strContents) const;
	bool ReadStringFromFileStream(std::string& outStrContents) const;

	bool IsOpenFileStream() const { return (m_pFileStream != nullptr); }

public:
	template <typename TData>
	bool WriteDataToFileStream(const TData& data) const
	{
		if (CheckValidFileStreamMode(EFileStreamMode::WriteOnlyBinary) == false)
		{
			return false;
		}

		int32 wroteCount = std::fwrite(&data, sizeof(TData), 1, m_pFileStream);
		if (wroteCount != 1)
		{
			return false;
		}

		return true;
	}

	template <typename TData>
	bool ReadDataFromFileStream(TData& outData) const
	{
		if (CheckValidFileStreamMode(EFileStreamMode::ReadOnlyBinary) == false)
		{
			return false;
		}

		int32 readCount = std::fread(&outData, sizeof(TData), 1, m_pFileStream);
		if (readCount != 1)
		{
			return false;
		}

		return true;
	}

private:
	bool CheckValidFileStreamMode(EFileStreamMode fileStreamMode) const;

private:
	std::string m_strFilePath;

	FILE* m_pFileStream = nullptr;
	EFileStreamMode m_fileStreamMode = EFileStreamMode::Count;
};