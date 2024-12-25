// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/Sprite/SpriteBase.h"

// 텍스처는 한장으로만 사용
class DynamicSprite : public SpriteBase
{
public:
	DynamicSprite();
	virtual ~DynamicSprite();

	void AddKeyFrame(int32 beginDrawPosX, int32 beginDrawPosY, int32 drawWidth, int32 drawHeight, float keepTime, uint32 colorKey);

	FORCEINLINE const SpriteDrawInfo& GetSpriteDrawInfo(int32 spriteDrawInfoIdx) const
	{
		VALIDATE_INDEX_RANGE(spriteDrawInfoIdx, m_vecSpriteDrawInfo.size());
		return m_vecSpriteDrawInfo[spriteDrawInfoIdx];
	}

	FORCEINLINE float GetSpriteKeyFrameTime(int32 spriteKeyFrameIdx) const
	{
		VALIDATE_INDEX_RANGE(spriteKeyFrameIdx, m_vecTimeTable.size());
		return m_vecTimeTable[spriteKeyFrameIdx];
	}
	
	const std::vector<float>& GetTimeTable() const { return m_vecTimeTable; }
	int32 GetSpriteKeyFrameCount() const { return m_vecSpriteDrawInfo.size(); }

private:
	std::vector<float> m_vecTimeTable; // 0번 인덱스에 2.0이 있다면 1번 인덱스까지 2초가 필요하다는 것
	std::vector<SpriteDrawInfo> m_vecSpriteDrawInfo;
};