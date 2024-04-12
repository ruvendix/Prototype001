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
	// �� ���� ��� ���ϱ�
	char szAppPath[MAX_PATH];
	::GetModuleFileName(nullptr, szAppPath, MAX_PATH);

	std::filesystem::path tempPath = szAppPath;
	m_strAppPath = tempPath.generic_string();

	tempPath = tempPath.parent_path().parent_path() / "Assets"; // 2�� �ö�
	tempPath += "\\";
	m_strAssetsDirectory = tempPath.generic_string();
}