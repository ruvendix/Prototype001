// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

#include "EngineSources/Global/Core.h"
#include "EngineSources/Global/DataTypes.h"
#include "EngineSources/Global/Constants.h"
#include "EngineSources/Global/ICoreLoop.h"
#include "EngineSources/Global/EnableSharedClass.h"
#include "EngineSources/Global/Macros.h"
#include "EngineSources/Global/Utility.hpp"

#include "EngineSources/UtilityClass/EnumBitset.hpp"
#include "EngineSources/UtilityClass/Delegator.hpp"
#include "EngineSources/UtilityClass/SortVector.hpp"
#include "EngineSources/UtilityClass/StringUtility.hpp"

#include "EngineSources/File/FileStream.h"
#include "EngineSources/Log/LogInclusion.h"

#include "EngineSources/Math/MathUtility.h"
#include "EngineSources/Math/Size.h"
#include "EngineSources/Math/Position2d.h"
#include "EngineSources/Math/Vector2d.h"

#include "EngineSources/Event/Event.hpp"
#include "EngineSources/ErrorHandler/ErrorHandler.h"

#include "EngineSources/Time/TimeUtility.h"
#include "EngineSources/Time/TimerManager.h"

#include "EngineSources/World/WorldContext.h"

#include "EngineSources/Component/Component.h"
#include "EngineSources/Component/TransformComponent.h"
#include "EngineSources/Component/CellActorMoveComponent.h"
#include "EngineSources/Component/CameraComponent.h"
#include "EngineSources/Component/RenderComponent/RenderComponent.h"
#include "EngineSources/Component/RenderComponent/StaticSpriteComponent.h"
#include "EngineSources/Component/RenderComponent/DynamicSpriteComponent.h"

#include "EngineSources/Actor/ActorDataTypes.h"
#include "EngineSources/Actor/Actor.h"
#include "EngineSources/Actor/CameraActor.h"
#include "EngineSources/Actor/CellActor.h"
#include "EngineSources/Actor/EffectActor/EffectActor.h"
#include "EngineSources/Actor/PawnActor/PawnActor.h"
#include "EngineSources/Actor/WorldTileMapActor.h"