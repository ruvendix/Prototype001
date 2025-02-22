// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "DynamicSprite.h"

DynamicSprite::DynamicSprite()
{

}

DynamicSprite::~DynamicSprite()
{

}

void DynamicSprite::AddKeyFrame(const SpriteDrawInfo& spriteDrawInfo, float keepTime)
{
	m_vecSpriteDrawInfo.push_back(spriteDrawInfo);

	// 유지시간이 0초인 건 패스
	if (math::CheckAlikeValue(keepTime, 0.0f) == true)
	{
		return;
	}

	if (m_vecTimeTable.empty() == true)
	{
		m_vecTimeTable.push_back(keepTime);
	}
	else
	{
		float endKeyFrameTime = m_vecTimeTable.back();
		float nextKeyFrameTime = (endKeyFrameTime + keepTime);
		m_vecTimeTable.push_back(nextKeyFrameTime);
	}
}

void DynamicSprite::AddKeyFrames(int32 startIdx, int32 endIdx, int32 spriteLine, const Size& drawSize, uint32 colorKey, float keepTime)
{
	for (int32 i = startIdx; i <= endIdx; ++i)
	{
		SpriteDrawInfo spriteDrawInfo;
		spriteDrawInfo.drawingStartPos = Position2d(i * drawSize.width, spriteLine * drawSize.height);
		spriteDrawInfo.drawSize = drawSize;
		spriteDrawInfo.colorKey = colorKey;

		AddKeyFrame(spriteDrawInfo, keepTime);
	}
}