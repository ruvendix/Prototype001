// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/Sprite/SpriteBase.h"

// �ؽ�ó�� �������θ� ���
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
	std::vector<float> m_vecTimeTable; // 0�� �ε����� 2.0�� �ִٸ� 1�� �ε������� 2�ʰ� �ʿ��ϴٴ� ��
	std::vector<SpriteDrawInfo> m_vecSpriteDrawInfo;
};