#pragma once

#include "ResourceBase.h"

struct SpriteInfo
{
	Int2d startPos;
	Size size;
	TexturePtr spTex = nullptr;
	uint32 excludeColor = 0;
};

class Sprite : public ResourceBase
{
public:
	Sprite(const std::string& strResPath);
	virtual ~Sprite();

	virtual void SaveResource() override;
	virtual void LoadResource() override;

	void AddSpriteInfo(const SpriteInfo& spriteInfo);

	const SpriteInfo* GetSpriteInfo(uint32 spriteInfoIdx) const { return m_spriteInfos.at(spriteInfoIdx); }
	uint32 GetSpriteInfoCount() const { return m_spriteInfos.size(); }

private:
	std::vector<SpriteInfo*> m_spriteInfos;
};