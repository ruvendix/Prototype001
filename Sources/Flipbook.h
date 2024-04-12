#pragma once

#include "ResourceBase.h"
#include "Sprite.h"

struct FlipbookData
{
	std::string strSpriteKey;
	TexturePtr spTex = nullptr;
	uint32 excludeColorKey = 0;
	int2d tileCount;
	Point2d tileStartPos;
	uint32 keyframeCount = 0;
	float keyframeInterval = 0.0f;
};

class Flipbook : public ResourceBase
{
public:
	Flipbook();
	Flipbook(SpritePtr spSprite);
	virtual ~Flipbook();

	virtual void LoadResource(const std::string& strPath) override;

	void  CreateKeyframes(const std::string& strSpriteKey, const FlipbookData& flipbookData);
	void  AddKeyframeTime(uint32 keyFrameIdx, float keepTime);
	const SpriteInfo* CalcKeyframe(float localTime) const;

	float GetFlipbookEndTime() const { return m_flipbookEndTime; }
	
private:
	SpritePtr m_spSprite = nullptr;
	std::vector<float> m_timeTable;
	float m_flipbookEndTime = 0.0f;
};