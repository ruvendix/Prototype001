// Copyright 2024 Ruvendix, All Rights Reserved.
#include "Pch.h"
#include "DynamicSprite.h"

DynamicSprite::DynamicSprite()
{

}

DynamicSprite::~DynamicSprite()
{

}

void DynamicSprite::AddKeyFrame(int32 beginDrawPosX, int32 beginDrawPosY, int32 drawWidth, int32 drawHeight, float keepTime)
{
	SpriteDrawInfo spriteDrawInfo;
	spriteDrawInfo.beginDrawPos = Position2d{ beginDrawPosX, beginDrawPosY };
	spriteDrawInfo.drawSize = Size{ drawWidth, drawHeight };
	m_vecSpriteDrawInfo.push_back(spriteDrawInfo);

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