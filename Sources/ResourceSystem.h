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

	SpritePtr CreateSprite(const std::string& strSpriteKey);
	const SpritePtr FindSprite(const std::string& strSpriteKey) const;

	FlipbookPtr CreateFlipbook(const std::string& strFlipbookKey);
	FlipbookPtr CreateFlipbook(const std::string& strFlipbookKey, SpritePtr spSprite); // 외부에서 스프라이트를 세팅한 경우
	const FlipbookPtr FindFlipbook(const std::string& strFlipbookKey) const;

private:
	std::unordered_map<std::string, TexturePtr>  m_texes;
	std::unordered_map<std::string, SpritePtr>   m_sprites;
	std::unordered_map<std::string, FlipbookPtr> m_Flipbooks;
};