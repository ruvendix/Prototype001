// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

using RenderComponentVector = std::vector<class RenderComponent*>;

class RenderComponent : public Component
{
	// ���� ������Ʈ�� Id�� �Ҵ�
	DECLARE_COMPILETIME_ID(RenderComponent)

public:
	RenderComponent();
	virtual ~RenderComponent();

	virtual void Render(HDC hBackBufferDc) = 0;
};