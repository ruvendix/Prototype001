#pragma once

#include "Defines.h"
#include "CompilerCounter.hpp"

DEFINE_COMPILER_COUNTER(ComponentIdCounter);

DEFINE_ID(ComponentIdCounter, TransformComponent);
DEFINE_ID(ComponentIdCounter, TextureComponent);
DEFINE_ID(ComponentIdCounter, FlipbookComponent);
DEFINE_ID(ComponentIdCounter, CameraComponent);
DEFINE_ID(ComponentIdCounter, CollisionComponent);