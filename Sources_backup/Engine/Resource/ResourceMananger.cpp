// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "ResourceMananger.h"

DEFINE_SINGLETON(ResourceMananger);

void ResourceMananger::Startup()
{
	m_spTexStorage = std::make_shared<TextureStorage>();
	m_spStaticSpriteStorage = std::make_shared<StaticSpriteStorage>();
	m_spDynamicSpriteStorage = std::make_shared<DynamicSpriteStorage>();
}

TexturePtr ResourceMananger::LoadTexture(const std::string& strResKey)
{
	ASSERT_LOG(m_spTexStorage != nullptr);
	return (std::dynamic_pointer_cast<Texture>(m_spTexStorage->LoadResource(strResKey)));
}

const Texture* ResourceMananger::FindTexture(const std::string& strResKey) const
{
	ASSERT_LOG(m_spTexStorage != nullptr);
	return (dynamic_cast<const Texture*>(m_spTexStorage->SearchResource(strResKey)));
}

// 캐스팅할 때 임시객체가 생성되므로 참조 반환 불가능
TexturePtr ResourceMananger::FindSharedTexture(const std::string& strResKey) const
{
	ASSERT_LOG(m_spTexStorage != nullptr);
	return (std::dynamic_pointer_cast<Texture>(m_spTexStorage->SearchSharedResource(strResKey)));
}

DynamicSpritePtr ResourceMananger::FindDynamicSprite(const std::string& strResKey) const
{
	ASSERT_LOG(m_spDynamicSpriteStorage != nullptr);
	return (std::dynamic_pointer_cast<DynamicSprite>(m_spDynamicSpriteStorage->SearchSharedResource(strResKey)));
}

StaticSpritePtr ResourceMananger::CreateStaticSprite(const std::string& strStaticSpriteTag) const
{
	ASSERT_LOG(m_spStaticSpriteStorage != nullptr);
	return (std::dynamic_pointer_cast<StaticSprite>(m_spStaticSpriteStorage->CreateResource(strStaticSpriteTag)));
}

DynamicSpritePtr ResourceMananger::CreateDynamicSprite(const std::string& strDynamicSpriteTag) const
{
	ASSERT_LOG(m_spDynamicSpriteStorage != nullptr);
	return (std::dynamic_pointer_cast<DynamicSprite>(m_spDynamicSpriteStorage->CreateResource(strDynamicSpriteTag)));
}

// 똑같이 Id 적용하면 가능하긴한데?