// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class RectangleShapeComponent : public RenderComponent
{
	DECLARE_COMPILETIME_ID;

public:
	RectangleShapeComponent();
	virtual ~RectangleShapeComponent();

	virtual void Render(HDC hBackBufferDc) override;
};