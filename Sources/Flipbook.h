/*
이미 로드된 텍스처를 이용해서 스프라이트로 만듬
*/

#pragma once
#include "ResourceBase.h"
#include "Sprite.h"

struct FlipbookData
{
	std::string  strSpriteKey;
	Texture::Ptr spTex = nullptr;
	uint32       excludeColorKey = 0;
	int2d        tileCount;
	Point2d      tileStartPos;
	uint32       keyframeCount = 0;
	float        keyframeInterval = 0.0f;
};

class Flipbook : public ResourceBase
{
public:
	DEFINE_SMART_PTR(Flipbook);

public:
	Flipbook();
	Flipbook(Sprite::Ptr spSprite);
	virtual ~Flipbook();

	virtual void LoadResource(const std::string& strPath) override;

	void  CreateKeyframes(const std::string& strSpriteKey, const FlipbookData& flipbookData);
	void  AddKeyframeTime(uint32 keyFrameIdx, float keepTime);
	const SpriteInfo* CalcKeyframe(float localTime) const;

	float GetFlipbookEndTime() const { return m_flipbookEndTime; }
	
private:
	Sprite::Ptr        m_spSprite = nullptr;
	std::vector<float> m_timeTable;
	float              m_flipbookEndTime = 0.0f;
};