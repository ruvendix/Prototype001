// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class FileStream
{
public:
	FileStream();
	~FileStream();

	bool OpenFileStream(const std::string& strFilePath);
	void CloseFileStream();

	bool ReopenFileStream();

	bool WriteToFileStream(const std::string& strContents);
	bool ReadFromFileStream(std::string& outStrContents);

	bool IsOpenFileStream() const { return (m_fileStream.is_open()); }

private:
	std::string m_strFilePath;
	std::fstream m_fileStream;
};