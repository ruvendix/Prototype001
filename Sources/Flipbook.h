#pragma once

#include "ResourceBase.h"
#include "Sprite.h"

class Flipbook : public ResourceBase
{
public:
	Flipbook(const std::string& strResPath);
	virtual ~Flipbook();

	virtual void SaveResource() override;
	virtual void LoadResource() override;

	void CreateTimeTable();
	void CreateKeyframes(const std::string strSprite, float keyframeInterval);

	void AddKeyframeTime(uint32 keyFrameIdx, float keepTime);
	const SpriteInfo* CalcKeyframe(float localTime) const;

	float GetFlipbookEndTime() const { return m_flipbookEndTime; }
	
private:
	// 파일에 쓰는 정보
	SpritePtr m_spSprite = nullptr;
	std::vector<float> m_keyframeIntervals;

	// 파일에 없어도 되는 정보
	float m_flipbookEndTime = 0.0f;
	std::vector<float> m_timeTable;
};