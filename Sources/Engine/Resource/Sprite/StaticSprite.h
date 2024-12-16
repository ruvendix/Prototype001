// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/Sprite/SpriteBase.h"

class StaticSprite : public SpriteBase
{
public:
	StaticSprite();
	virtual ~StaticSprite();

	void SetBeginDrawPos(int32 beginDrawPosX, int32 beginDrawPosY) { m_spriteDrawInfo.beginDrawPos = Position2d{ beginDrawPosX, beginDrawPosY }; }
	void SetBeginDrawPos(const Position2d& beginDrawPos) { m_spriteDrawInfo.beginDrawPos = beginDrawPos; }

	int32 GetBeginDrawPosX() const { return m_spriteDrawInfo.beginDrawPos.x; }
	int32 GetBeginDrawPosY() const { return m_spriteDrawInfo.beginDrawPos.y; }

	void SetDrawSize(const Size& drawSize) { m_spriteDrawInfo.drawSize = drawSize; }
	void SetDrawSize(int32 drawWidth, int32 drawHeight) { m_spriteDrawInfo.drawSize = Size{ drawWidth, drawHeight }; }

	int32 GetDrawWidth() const { return m_spriteDrawInfo.drawSize.width; }
	int32 GetDrawHeight() const { return m_spriteDrawInfo.drawSize.height; }

	const SpriteDrawInfo& GetSpriteDrawInfo() const { return m_spriteDrawInfo; }

private:
	SpriteDrawInfo m_spriteDrawInfo;
};