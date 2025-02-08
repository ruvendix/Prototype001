// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "ActorEnums.h"

using ActorStorage = std::unordered_map<EActorLayerType, Actors>;
using RenderableActorsArray = std::array<Actors, TO_NUM(EActorLayerType::Count)>;