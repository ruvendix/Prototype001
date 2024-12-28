// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/Sprite/SpriteBase.h"

class StaticSprite : public SpriteBase
{
public:
	StaticSprite();
	virtual ~StaticSprite();

	void SetSpriteDrawInfo(const SpriteDrawInfo& spriteDrawInfo) { m_spriteDrawInfo = spriteDrawInfo; }
	const SpriteDrawInfo& GetSpriteDrawInfo() const { return m_spriteDrawInfo; }

	void SetDrawingStartPos(const Position2d& drawingStartPos) { m_spriteDrawInfo.drawingStartPos = drawingStartPos; }

private:
	SpriteDrawInfo m_spriteDrawInfo;
};