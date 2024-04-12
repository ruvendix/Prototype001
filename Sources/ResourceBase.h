#pragma once

#include "IGameDefault.h"

class ResourceBase : IGameDefault
{
public:
	ResourceBase();
	virtual ~ResourceBase();

	virtual void Startup() override;
	virtual bool Update() override;
	virtual bool PostUpdate() override;
	virtual void Render() override;
	virtual void Cleanup() override;

	virtual void LoadResource(const std::string& strPath) = 0;
};