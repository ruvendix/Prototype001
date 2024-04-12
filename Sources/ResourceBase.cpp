#include "Pch.h"
#include "ResourceBase.h"

ResourceBase::ResourceBase(const std::string& strResPath)
{
	m_strResPath = strResPath;
}

ResourceBase::~ResourceBase()
{

}

void ResourceBase::Startup()
{

}

bool ResourceBase::Update()
{
	return true;
}

bool ResourceBase::PostUpdate()
{
	return true;
}

void ResourceBase::Render()
{

}

void ResourceBase::Cleanup()
{

}

void ResourceBase::AddFileExtension(const std::string strFileExt)
{
	m_strResPath += strFileExt;
}