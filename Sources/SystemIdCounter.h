#pragma once

#include "Defines.h"
#include "CompilerCounter.hpp"

DEFINE_COMPILER_COUNTER(SystemIdCounter);

DEFINE_ID(SystemIdCounter, FrameSystem);
DEFINE_ID(SystemIdCounter, InputSystem);
DEFINE_ID(SystemIdCounter, PathSystem);
DEFINE_ID(SystemIdCounter, ResourceSystem);
DEFINE_ID(SystemIdCounter, UiSystem);
DEFINE_ID(SystemIdCounter, SceneSystem);
DEFINE_ID(SystemIdCounter, CollisionSystem);