/*
경로를 다룸
*/

#include "Pch.h"
#include "PathSystem.h"

PathSystem::PathSystem()
{

}

PathSystem::~PathSystem()
{

}

void PathSystem::Startup()
{
	// 앱 기준 경로 구하기
	char szAppPath[MAX_PATH];
	::GetModuleFileName(nullptr, szAppPath, MAX_PATH);

	std::filesystem::path tempPath = szAppPath;
	m_strAppPath = tempPath.generic_string();

	tempPath = tempPath.parent_path().parent_path(); // 2번 올라감
	tempPath += "\\Assets\\";
	m_strAssetsDirectory = tempPath.generic_string();
}