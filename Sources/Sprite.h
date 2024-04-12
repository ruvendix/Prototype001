#pragma once

#include "ResourceBase.h"

struct SpriteInfo
{
	Point2d pos;
	Size size;
	TexturePtr spTex = nullptr;
	uint32 excludeColor = 0;
};

class Sprite : public ResourceBase
{
public:
	Sprite();
	virtual ~Sprite();

	// 스프라이트 정보를 파일에서 읽는 경우
	virtual void LoadResource(const std::string& strPath) override;

	void AddSpriteInfo(const SpriteInfo& spriteInfo);

	const SpriteInfo* GetSpriteInfo(uint32 spriteInfoIdx) const { return m_spriteInfos.at(spriteInfoIdx); }
	uint32 GetSpriteInfoCount() const { return m_spriteInfos.size(); }

private:
	std::vector<SpriteInfo*> m_spriteInfos;
};