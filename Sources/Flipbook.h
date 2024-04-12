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
	// ���Ͽ� ���� ����
	SpritePtr m_spSprite = nullptr;
	std::vector<float> m_keyframeIntervals;

	// ���Ͽ� ��� �Ǵ� ����
	float m_flipbookEndTime = 0.0f;
	std::vector<float> m_timeTable;
};