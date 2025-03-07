// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "ActorEnums.h"

using Actors = std::vector<ActorPtr>;
using LayerActors = std::array<Actors, TO_NUM(EActorLayerType::Count)>;