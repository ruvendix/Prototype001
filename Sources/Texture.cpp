/*
비트맵 리소스
*/

#include "Pch.h"
#include "Texture.h"

#include "GameApplication.h"

Texture::Texture()
{
}

Texture::~Texture()
{
	::DeleteObject(m_hBitMap);
	::ReleaseDC(nullptr, m_hDc);
}

void Texture::LoadResource(const std::string& strPath)
{
	m_hDc = GameApplication::I()->CreateCompatibleDc();
	m_hBitMap = static_cast<HBITMAP>(::LoadImage(nullptr, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
	
	HBITMAP prevBitMap = static_cast<HBITMAP>(::SelectObject(m_hDc, m_hBitMap));
	::DeleteObject(prevBitMap);

	BITMAP bitMapInfo;
	::ZeroMemory(&bitMapInfo, sizeof(BITMAP));
	::GetObject(m_hBitMap, sizeof(BITMAP), &bitMapInfo);

	m_size.width  = bitMapInfo.bmWidth;
	m_size.height = bitMapInfo.bmHeight;
}