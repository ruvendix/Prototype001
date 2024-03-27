/*
리소스를 다룸
*/

#include "Pch.h"
#include "ResourceSystem.h"

#include "SystemManager.h"
#include "PathSystem.h"

ResourceSystem::ResourceSystem()
{

}

ResourceSystem::~ResourceSystem()
{

}

void ResourceSystem::Cleanup()
{

}

Texture::Ptr ResourceSystem::LoadTexture(const std::string& strTexPath)
{
	Texture::Ptr spTex = FindTexture(strTexPath);
	if (spTex != nullptr)
	{
		return spTex;
	}

	// 키는 파일 경로
	std::filesystem::path texPath = GET_SYSTEM(PathSystem)->GetAssetsDirectory();
	texPath += strTexPath;

	// 텍스처 객체 생성 및 로드
	spTex = std::make_shared<Texture>();
	spTex->LoadResource(texPath.generic_string());

	m_texes.emplace(strTexPath, spTex);
	return spTex;
}

const Texture::Ptr ResourceSystem::FindTexture(const std::string& strTexPath) const
{
	auto foundIter = m_texes.find(strTexPath);
	if (foundIter == m_texes.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

Sprite::Ptr ResourceSystem::CreateSprite(const std::string& strSpriteKey)
{
	// 스프라이트 객체 생성 및 로드
	Sprite::Ptr spNewSprite = std::make_shared<Sprite>();
	m_sprites.emplace(strSpriteKey, spNewSprite);
	return spNewSprite;
}

const Sprite::Ptr ResourceSystem::FindSprite(const std::string& strSpriteKey) const
{
	auto foundIter = m_sprites.find(strSpriteKey);
	if (foundIter == m_sprites.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

Flipbook::Ptr ResourceSystem::CreateFlipbook(const std::string& strFlipbookKey)
{
	// 플립북 객체 생성 및 로드
	Flipbook::Ptr spNewFlipbook = std::make_shared<Flipbook>();
	m_Flipbooks.emplace(strFlipbookKey, spNewFlipbook);
	return spNewFlipbook;
}

Flipbook::Ptr ResourceSystem::CreateFlipbook(const std::string& strFlipbookKey, Sprite::Ptr spSprite)
{
	// 플립북 객체 생성 및 로드
	Flipbook::Ptr spNewFlipbook = std::make_shared<Flipbook>(spSprite);
	m_Flipbooks.emplace(strFlipbookKey, spNewFlipbook);
	return spNewFlipbook;
}

const Flipbook::Ptr ResourceSystem::FindFlipbook(const std::string& strFlipbookKey) const
{
	auto foundIter = m_Flipbooks.find(strFlipbookKey);
	if (foundIter == m_Flipbooks.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}