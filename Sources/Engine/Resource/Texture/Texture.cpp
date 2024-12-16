// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "Texture.h"

Texture::Texture()
{

}

Texture::~Texture()
{
	::DeleteObject(m_hBitMap);
	::ReleaseDC(nullptr, m_hTexDc);
}

void Texture::SaveResource()
{

}

void Texture::LoadResource(const std::string& strResRelativePath)
{
	m_hTexDc = SceneRenderer::I()->CreateCompatibleDc();
	m_hBitMap = static_cast<HBITMAP>(::LoadImage(nullptr, strResRelativePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));

	HBITMAP prevBitMap = static_cast<HBITMAP>(::SelectObject(m_hTexDc, m_hBitMap));
	::DeleteObject(prevBitMap);

	BITMAP bitMapInfo;
	::ZeroMemory(&bitMapInfo, sizeof(BITMAP));
	::GetObject(m_hBitMap, sizeof(BITMAP), &bitMapInfo);

	m_texSize.width = bitMapInfo.bmWidth;
	m_texSize.height = bitMapInfo.bmHeight;
}