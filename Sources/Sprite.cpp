/*
비트맵 리소스
*/

#include "Pch.h"
#include "Sprite.h"

#include "GameApplication.h"

Sprite::Sprite()
{

}

Sprite::~Sprite()
{
	FOR_EACH_SAFE_DELETE(m_spriteInfos);
}

void Sprite::LoadResource(const std::string& strPath)
{
	UNREFERENCED_PARAMETER(strPath);
}

void Sprite::AddSpriteInfo(const SpriteInfo& spriteInfo)
{
	SpriteInfo* pNewSpriteInfo = new SpriteInfo;
	*pNewSpriteInfo = spriteInfo;
	m_spriteInfos.push_back(pNewSpriteInfo);
}