// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using RenderComponentVector = std::vector<class RenderComponent*>;

class RenderComponent : public Component
{
	// ·»´õ ÄÄÆ÷³ÍÆ®µµ Id´Â ÇÒ´ç
	DECLARE_COMPILETIME_ID;

public:
	RenderComponent();
	virtual ~RenderComponent();

	Position2d CalculateRenderingStartPosition();

	virtual void Render(HDC hBackBufferDc) = 0;
};