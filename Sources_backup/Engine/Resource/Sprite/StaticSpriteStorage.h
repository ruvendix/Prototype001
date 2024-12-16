// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/ResourceStorage.h"
#include "StaticSprite.h"

class StaticSpriteStorage : public ResourceStorage<StaticSprite>
{
public:
	StaticSpriteStorage();
	virtual ~StaticSpriteStorage();
};