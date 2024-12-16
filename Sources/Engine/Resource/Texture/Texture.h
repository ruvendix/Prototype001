// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/PersistantResource.h"

class Texture : public PersistantResource
{
public:
	Texture();
	virtual ~Texture();

	virtual void SaveResource() override;
	virtual void LoadResource(const std::string& strResRelativePath) override;

	HDC GetTextureDc() const { return m_hTexDc; }
	const Size& GetTextureSize() const { return m_texSize; }

private:
	HDC m_hTexDc = nullptr;
	HBITMAP m_hBitMap = nullptr;
	Size m_texSize;
};