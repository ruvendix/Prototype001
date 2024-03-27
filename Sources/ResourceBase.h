/*
리소스의 기본 클래스
*/

#pragma once
#include "IDefault.h"

class ResourceBase : IDefault
{
public:
	ResourceBase();
	virtual ~ResourceBase();

	virtual void Startup() override;
	virtual bool Update()  override;
	virtual void Render()  override;
	virtual void Cleanup() override;

	virtual void LoadResource(const std::string& strPath) = 0;
};