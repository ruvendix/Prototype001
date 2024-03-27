/*
°æ·Î¸¦ ´Ù·ë
*/

#pragma once
#include "SystemBase.h"

class PathSystem : public SystemBase
{
public:
	PathSystem();
	virtual ~PathSystem();

	virtual void Startup() override;

	const std::string& GetAppPath() const { return m_strAppPath; }
	const std::string& GetAssetsDirectory() const { return m_strAssetsDirectory; }

private:
	std::string m_strAppPath;
	std::string m_strAssetsDirectory;
};