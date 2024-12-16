// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "DataTypes.h"
#include "Macros.h"
#include "IGameLoop.h"
#include "UtilityFunctions.h"

#include "Engine/String/StringUtility.hpp"
#include "Engine/Time/TimeUtility.h"

#include "Engine/UtilityClass/EnumBitset.h"
#include "Engine/Log/LogInclusion.h"

#include "Engine/ErrorHandler/ErrorHandler.h"
#include "Engine/Time/TimerManager.h"
#include "Engine/GameFrame/GameFrameHandler.h"
#include "Engine/UserInput/KeyboardInputHandler.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/RenderComponent.h"
#include "Engine/Component/StaticSpriteComponent.h"
#include "Engine/Component/DynamicSpriteComponent.h"
#include "Engine/Component/SceneActorMoveComponent.h"

#include "Engine/Actor/Actor.h"
#include "Engine/Actor/SceneActor.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneTracer.h"
#include "Engine/Scene/SceneRenderer.h"

#include "Engine/Event/EventManager.h"
#include "Engine/Event/EventListener.h"

#include "Engine/Resource/ResourceMananger.h"
#include "Engine/Scene/SceneManager.h"