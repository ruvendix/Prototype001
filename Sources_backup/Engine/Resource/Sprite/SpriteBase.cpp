// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "SpriteBase.h"

SpriteBase::SpriteBase()
{

}

SpriteBase::~SpriteBase()
{

}

void SpriteBase::SaveResource()
{

}

void SpriteBase::LoadResource(const std::string& strResRelativePath)
{

}

void SpriteBase::FindAndSetTexture(const std::string& strResRelativePath)
{
	const TexturePtr& spFoundTex = ResourceMananger::I()->FindSharedTexture(strResRelativePath);
	if (spFoundTex == nullptr)
	{
		DETAIL_ERROR_LOG(EErrorCode::NotFoundResource);
		return;
	}

	SetTexture(spFoundTex);
}