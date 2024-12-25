// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/PersistantResource.h"

struct SpriteDrawInfo
{
	Position2d beginDrawPos;
	Size drawSize;
	uint32 colorKey = 0;
};

class SpriteBase : public PersistantResource
{
public:
	SpriteBase();
	virtual ~SpriteBase();

	virtual void SaveResource() override;
	virtual void LoadResource(const std::string& strResRelativePath) override;

	void LoadAndSetTexture(const std::string& strResRelativePath);
	void FindAndSetTexture(const std::string& strResRelativePath);

	void SetTexture(const TexturePtr& spTex) { m_spTex = spTex; }
	const Texture* GetTexture() const { return (m_spTex.get()); }

private:
	TexturePtr m_spTex = nullptr;
};