// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/File/FileStream.h"

class PersistantResource
{
public:
	PersistantResource();
	virtual ~PersistantResource();

	virtual void SaveResource() = 0;
	virtual void LoadResource(const std::string& strResRelativePath) = 0;

	void SetResourceRelativePath(const std::string& strResRelativePath) { m_strResRelativePath = strResRelativePath; }
	const std::string& GetResourceRelativePath() const { return m_strResRelativePath; }
	
	const std::string& GetTag() const { return m_strResTag; }
	void SetTag(const std::string& strResTag) { m_strResTag = strResTag; }

private:
	std::string m_strResTag; // �޸𸮿����� ����ϸ� �±׷� �ĺ� (�ʼ��� �ƴ�)
	std::string m_strResRelativePath;
	FileStream m_fileStream;
};