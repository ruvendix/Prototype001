// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/ResourceStorage.h"
#include "DynamicSprite.h"

class DynamicSpriteStorage : public ResourceStorage<DynamicSprite>
{
public:
	DynamicSpriteStorage();
	virtual ~DynamicSpriteStorage();
};