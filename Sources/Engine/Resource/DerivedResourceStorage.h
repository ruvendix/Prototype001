// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/ResourceStorage.h"
#include "Engine/Resource/Texture/Texture.h"
#include "Engine/Resource/Sprite/StaticSprite.h"
#include "Engine/Resource/Sprite/DynamicSprite.h"
#include "Engine/Resource/Effect/Effect.h"

class TextureStorage : public ResourceStorage<Texture>
{
public:
	TextureStorage() = default;
	virtual ~TextureStorage() = default;
};

class StaticSpriteStorage : public ResourceStorage<StaticSprite>
{
public:
	StaticSpriteStorage() = default;
	virtual ~StaticSpriteStorage() = default;
};

class DynamicSpriteStorage : public ResourceStorage<DynamicSprite>
{
public:
	DynamicSpriteStorage() = default;
	virtual ~DynamicSpriteStorage() = default;
};

class EffectStorage : public ResourceStorage<Effect>
{
public:
	EffectStorage() = default;
	virtual ~EffectStorage() = default;
};