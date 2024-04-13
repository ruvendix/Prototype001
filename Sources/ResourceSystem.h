#pragma once

#include "SystemBase.h"

class ResourceSystem : public SystemBase
{
public:
	ResourceSystem();
	virtual ~ResourceSystem();

	virtual void Cleanup() override;

	TexturePtr LoadTexture(const std::string& strTexPath);
	const TexturePtr FindTexture(const std::string& strTexPath) const;

	SpritePtr CreateSprite(const std::string& strSpritePath);
	const SpritePtr FindSprite(const std::string& strSpritePath) const;
	SpritePtr LoadSprite(const std::string& strSpritePath);

	FlipbookPtr CreateFlipbook(const std::string& strFlipbookPath);
	const FlipbookPtr FindFlipbook(const std::string& strFlipbookPath) const;
	FlipbookPtr LoadFlipbook(const std::string& strFlipbookPath);

	TileMapPtr LoadTileMap(const std::string& strTileMapPath);
	TileMapPtr GetTileMap() const { return m_spTileMap; }

private:
	std::unordered_map<std::string, TexturePtr>  m_texes;
	std::unordered_map<std::string, SpritePtr>   m_sprites;
	std::unordered_map<std::string, FlipbookPtr> m_Flipbooks;

	// 타일맵은 일단 하나로 퉁침
	TileMapPtr m_spTileMap = nullptr;
};