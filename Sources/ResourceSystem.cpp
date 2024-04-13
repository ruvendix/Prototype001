#include "Pch.h"
#include "ResourceSystem.h"

#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "TileMap.h"

ResourceSystem::ResourceSystem()
{

}

ResourceSystem::~ResourceSystem()
{

}

void ResourceSystem::Cleanup()
{
	//for (auto& iter : m_sprites)
	//{
	//	iter.second->SaveResource();
	//}

	//for (auto& iter : m_Flipbooks)
	//{
	//	iter.second->SaveResource();
	//}

	m_spTileMap->SaveResource();
}

TexturePtr ResourceSystem::LoadTexture(const std::string& strTexPath)
{
	TexturePtr spFoundTex = FindTexture(strTexPath);
	if (spFoundTex != nullptr)
	{
		return spFoundTex;
	}

	TexturePtr spNewTex = std::make_shared<Texture>(strTexPath);
	spNewTex->LoadResource();
	m_texes.emplace(strTexPath, spNewTex);

	return spNewTex;
}

const TexturePtr ResourceSystem::FindTexture(const std::string& strTexPath) const
{
	auto foundIter = m_texes.find(strTexPath);
	if (foundIter == m_texes.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

SpritePtr ResourceSystem::CreateSprite(const std::string& strSpritePath)
{
	// 스프라이트 객체 생성 및 로드
	SpritePtr spNewSprite = std::make_shared<Sprite>(strSpritePath);
	m_sprites.emplace(strSpritePath, spNewSprite);
	return spNewSprite;
}

const SpritePtr ResourceSystem::FindSprite(const std::string& strSpritePath) const
{
	auto foundIter = m_sprites.find(strSpritePath);
	if (foundIter == m_sprites.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

SpritePtr ResourceSystem::LoadSprite(const std::string& strSpritePath)
{
	SpritePtr spFoundSprite = FindSprite(strSpritePath);
	if (spFoundSprite != nullptr)
	{
		return spFoundSprite;
	}

	SpritePtr spNewSprite = CreateSprite(strSpritePath);
	spNewSprite->LoadResource();

	return spNewSprite;
}

FlipbookPtr ResourceSystem::CreateFlipbook(const std::string& strFlipbookPath)
{
	// 플립북 객체 생성 및 로드
	FlipbookPtr spNewFlipbook = std::make_shared<Flipbook>(strFlipbookPath);
	m_Flipbooks.emplace(strFlipbookPath, spNewFlipbook);
	return spNewFlipbook;
}

const FlipbookPtr ResourceSystem::FindFlipbook(const std::string& strFlipbookPath) const
{
	auto foundIter = m_Flipbooks.find(strFlipbookPath);
	if (foundIter == m_Flipbooks.cend())
	{
		return nullptr;
	}

	return (foundIter->second);
}

FlipbookPtr ResourceSystem::LoadFlipbook(const std::string& strFlipbookPath)
{
	FlipbookPtr spFoundFlipbook = FindFlipbook(strFlipbookPath);
	if (spFoundFlipbook != nullptr)
	{
		return spFoundFlipbook;
	}

	FlipbookPtr spNewFlipbook = CreateFlipbook(strFlipbookPath);
	spNewFlipbook->LoadResource();

	return spNewFlipbook;
}

TileMapPtr ResourceSystem::LoadTileMap(const std::string& strTileMapPath)
{
	m_spTileMap.reset();

	m_spTileMap = std::make_shared<TileMap>(strTileMapPath);
	m_spTileMap->LoadResource();

	return m_spTileMap;
}