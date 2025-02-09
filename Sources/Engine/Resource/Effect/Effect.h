// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/PersistantResource.h"
#include "Engine/Resource/Effect/EffectStructs.h"

// 애초에 이거 만들 때 라이프타임인지 단발인지 여기서 결정해야함!
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
	// 이게 이미 완성된 이펙트니까 리소스 관련만 있어야함
	DynamicSpritePtr m_spDynamicSprite = nullptr; // 재활용이 아닌 복사해서 사용 (재활용하면 기존 DynamicSprite도 변경됨)
	float m_lifeTime = 0.0f;
};