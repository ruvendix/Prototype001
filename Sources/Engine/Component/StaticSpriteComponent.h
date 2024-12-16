// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class StaticSpriteComponent : public RenderComponent
{
	// ·»´õ ÄÄÆ÷³ÍÆ®µµ Id´Â ÇÒ´ç
	DECLARE_COMPILETIME_ID(StaticSpriteComponent)

public:
	StaticSpriteComponent();
	virtual ~StaticSpriteComponent();

	virtual void Render(HDC hBackBufferDc) override;

	void SetStaticSprite(const StaticSpritePtr& spStaticSprite) { m_spStaticSprite = spStaticSprite; }

private:
	StaticSpritePtr m_spStaticSprite = nullptr;
};