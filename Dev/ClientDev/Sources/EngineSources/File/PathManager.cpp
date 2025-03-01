// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "PathManager.h"

namespace
{
	std::filesystem::path g_nullFilePath;
}

DEFINE_SINGLETON(PathManager);

void PathManager::AddPathAtCurrentWorkingDirectory(const std::string& strRelativePath)
{
	// 항상 있는지부터 확인
	if (FindPath(strRelativePath).empty() == false)
	{
		return;
	}

	std::filesystem::path newPath = std::filesystem::current_path();
	newPath.append(strRelativePath);
	newPath += "\\";

	if (std::filesystem::is_directory(newPath) == false)
	{
		std::lock_guard lockGuard(m_mutex);
		if (std::filesystem::create_directories(newPath) == false)
		{
			// 오류
		}
	}

	AddPath(strRelativePath, newPath);
}

void PathManager::AddPath(const std::string& strKey, const std::filesystem::path& filePath)
{
	auto insertedIter = m_mapPath.insert(std::make_pair(strKey, filePath));
	if (insertedIter.second == false)
	{
		// 오류
	}
}

const std::filesystem::path& PathManager::FindPath(const std::string& strKey)
{
	auto foundIter = m_mapPath.find(strKey);
	if (foundIter == m_mapPath.cend())
	{
		return g_nullFilePath;
	}

	return (foundIter->second);
}