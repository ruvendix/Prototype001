// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using RenderComponentVector = std::vector<class RenderComponent*>;
using RenderComponentVectorArray = std::array<RenderComponentVector, TO_NUM(ERenderingLayerType::Count)>;

class RenderComponent : public Component
{
	// ���� ������Ʈ�� Id�� �Ҵ�
	DECLARE_COMPILETIME_ID(RenderComponent)

public:
	RenderComponent();
	virtual ~RenderComponent();

	Position2d CalculateRenderingStartPosition();

	virtual void Render(HDC hBackBufferDc) = 0;
};