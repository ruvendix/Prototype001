#pragma once

#include "IGameDefault.h"

class ResourceBase : IGameDefault
{
public:
	ResourceBase(const std::string& strResPath);
	virtual ~ResourceBase();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	virtual void SaveResource() = 0;
	virtual void LoadResource() = 0;

	void AddFileExtension(const std::string strFileExt);

	const std::string& GetResourcePath() const { return m_strResPath; }

private:
	std::string m_strResPath;
};