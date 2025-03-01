// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "DataTypes.h"
#include "Macros.h"
#include "ICoreLoop.h"
#include "EnableSharedClass.h"
#include "Utility.hpp"

#include "EngineSources/SpecialContainer/SortVector.hpp"

#include "EngineSources/UtilityClass/EnumBitset.hpp"
#include "EngineSources/String/StringUtility.hpp"

#include "EngineSources/Log/LogInclusion.h"
#include "EngineSources/ErrorHandler/ErrorHandler.h"

#include "EngineSources/Math/MathUtility.h"
#include "EngineSources/Math/Size.h"
#include "EngineSources/Math/Vector2d.h"
#include "EngineSources/Math/Position2d.h"

#include "EngineSources/File/FileStream.h"

#include "EngineSources/Delegator/Delegator.hpp"
#include "EngineSources/Event/Event.hpp"

#include "EngineSources/Time/TimeUtility.h"
#include "EngineSources/Time/TimerManager.h"
#include "EngineSources/GameFrame/GameFrameHandler.h"

#include "EngineSources/Input/InputDataTypes.h"
#include "EngineSources/Input/InputActionValue.h"
#include "EngineSources/Input/InputAction.h"
#include "EngineSources/Input/InputMappingContext.h"
#include "EngineSources/Input/InputDeviceManager.h"
#include "EngineSources/Input/KeyboardDevice.h"
#include "EngineSources/Input/MouseDevice.h"

#include "EngineSources/LocalPlayer/LocalPlayerInputSystem.h"
#include "EngineSources/World/WorldContext.h"

#include "EngineSources/Component/Component.h"
#include "EngineSources/Component/TransformComponent.h"
#include "EngineSources/Component/CellActorMoveComponent.h"
#include "EngineSources/Component/EditorActorMoveComponent.h"
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

#include "EngineSources/Scene/Scene.h"
#include "EngineSources/Scene/SceneTracer.h"
#include "EngineSources/Scene/SceneRenderer.h"

#include "EngineSources/Resource/ResourceMananger.h"
#include "EngineSources/Scene/SceneManager.h"