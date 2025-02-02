// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/Sprite/SpriteBase.h"

// 텍스처는 한장으로만 사용
class DynamicSprite : public SpriteBase
{
public:
	DynamicSprite();
	virtual ~DynamicSprite();

	void AddKeyFrame(const SpriteDrawInfo& spriteDrawInfo, float keepTime);
	void AddKeyFrames(int32 startIdx, int32 endIdx, int32 spriteLine, const Size& drawSize, uint32 colorKey, float keepTime);

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

	void SetLoopDynamicSprite(bool bLoop) { m_bLoop = bLoop; }
	bool IsLoopDynamicSprite() const { return (m_bLoop == true); }
	
	const std::vector<float>& GetTimeTable() const { return m_vecTimeTable; }
	int32 GetSpriteKeyFrameCount() const { return m_vecSpriteDrawInfo.size(); }

private:
	std::vector<float> m_vecTimeTable; 
	std::vector<SpriteDrawInfo> m_vecSpriteDrawInfo; // 이거 하나하나가 키프레임
	bool m_bLoop = false;
};