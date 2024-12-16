// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class PathManager
{
	DECLARE_SINGLETON(PathManager)

public:
	void AddPathAtCurrentWorkingDirectory(const std::string& strRelativePath);

	void AddPath(const std::string& strKey, const std::filesystem::path& filePath);
	const std::filesystem::path& FindPath(const std::string& strKey);
	
private:
	std::mutex m_mutex;
	std::unordered_map<std::string, std::filesystem::path> m_mapPath;
};