// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Texture/TextureStorage.h"
#include "Sprite/StaticSpriteStorage.h"
#include "Sprite/DynamicSpriteStorage.h"

class ResourceMananger : public IGameLoop
{
	DECLARE_SINGLETON(ResourceMananger)

public:
	virtual void Startup() override;

	TexturePtr LoadTexture(const std::string& strResKey);

	const Texture* FindTexture(const std::string& strResKey) const;
	TexturePtr FindSharedTexture(const std::string& strResKey) const;
	DynamicSpritePtr FindDynamicSprite(const std::string& strResKey) const;

	StaticSpritePtr CreateStaticSprite(const std::string& strStaticSpriteTag) const;
	DynamicSpritePtr CreateDynamicSprite(const std::string& strDynamicSpriteTag) const;

private:
	std::shared_ptr<TextureStorage> m_spTexStorage = nullptr;
	std::shared_ptr<StaticSpriteStorage> m_spStaticSpriteStorage = nullptr;
	std::shared_ptr<DynamicSpriteStorage> m_spDynamicSpriteStorage = nullptr;
};