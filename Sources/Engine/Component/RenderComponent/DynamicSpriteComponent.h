// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

class DynamicSpriteComponent : public RenderComponent
{
	DECLARE_COMPILETIME_ID;

public:
	DynamicSpriteComponent();
	virtual ~DynamicSpriteComponent();

	virtual bool Update(float deltaSeconds) override;
	virtual void Render(HDC hBackBufferDc) override;

	void ResetDynamicSprite();
	void ApplyDynamicSprite(const DynamicSpritePtr& spDynamicSprite);

private:
	// Ÿ�̸Ӹ� ������� ����
	bool m_bLoop = true; // �⺻������ ��������Ʈ �ִϸ��̼��� �ݺ�
	float m_localTime = 0.0f;

	int32 m_spriteKeyFrameCount = 0;
	int32 m_currentSpriteDrawInfoIdx = 0;
	DynamicSpritePtr m_spDynamicSprite = nullptr;
};