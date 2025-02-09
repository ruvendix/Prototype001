// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/PersistantResource.h"
#include "Engine/Resource/Effect/EffectStructs.h"

// ���ʿ� �̰� ���� �� ������Ÿ������ �ܹ����� ���⼭ �����ؾ���!
class Effect : public PersistantResource
{
public:
	Effect();
	virtual ~Effect();

	virtual void SaveResource() override;
	virtual void LoadResource(const std::string& strResRelativePath) override;

public:
	void SetDynamicSprite(const DynamicSpritePtr& spDynamicSprite) { m_spDynamicSprite = spDynamicSprite; }
	const DynamicSpritePtr& GetDynamicSprite() const { return m_spDynamicSprite; }

	void SetLifeTime(float lifeTime) { m_lifeTime = lifeTime; }

private:
	// �̰� �̹� �ϼ��� ����Ʈ�ϱ� ���ҽ� ���ø� �־����
	DynamicSpritePtr m_spDynamicSprite = nullptr; // ��Ȱ���� �ƴ� �����ؼ� ��� (��Ȱ���ϸ� ���� DynamicSprite�� �����)
	float m_lifeTime = 0.0f;
};