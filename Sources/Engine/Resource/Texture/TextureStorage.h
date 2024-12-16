// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "Engine/Resource/ResourceStorage.h"
#include "Texture.h"

class TextureStorage : public ResourceStorage<Texture>
{
public:
	TextureStorage();
	virtual ~TextureStorage();
};