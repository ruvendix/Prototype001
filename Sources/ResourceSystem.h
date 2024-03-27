/*
리소스를 다룸
*/

#pragma once
#include "SystemBase.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"

class ResourceSystem : public SystemBase
{
public:
	ResourceSystem();
	virtual ~ResourceSystem();

	virtual void Cleanup() override;

	Texture::Ptr LoadTexture(const std::string& strTexPath);
	const Texture::Ptr FindTexture(const std::string& strTexPath) const;

	Sprite::Ptr CreateSprite(const std::string& strSpriteKey);
	const Sprite::Ptr FindSprite(const std::string& strSpriteKey) const;

	Flipbook::Ptr CreateFlipbook(const std::string& strFlipbookKey);
	Flipbook::Ptr CreateFlipbook(const std::string& strFlipbookKey, Sprite::Ptr spSprite); // 외부에서 스프라이트를 세팅한 경우
	const Flipbook::Ptr FindFlipbook(const std::string& strFlipbookKey) const;

private:
	std::unordered_map<std::string, Texture::Ptr>  m_texes;
	std::unordered_map<std::string, Sprite::Ptr>   m_sprites;
	std::unordered_map<std::string, Flipbook::Ptr> m_Flipbooks;
};