// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

#include "DataTypes.h"
#include "Enums.h"
#include "Macros.h"
#include "ICoreLoop.h"
#include "Utility.h"

#include "Engine/SpecialContainer/SortVector.hpp"

#include "Engine/UtilityClass/EnumBitset.hpp"
#include "Engine/String/StringUtility.hpp"

#include "Engine/Log/LogInclusion.h"
#include "Engine/ErrorHandler/ErrorHandler.h"

#include "Engine/Math/MathUtility.h"
#include "Engine/Math/Size.h"

#include "Engine/File/FileStream.h"

#include "Engine/Delegator/Delegator.hpp"
#include "Engine/Event/Event.hpp"

#include "Engine/Time/TimeUtility.h"
#include "Engine/Time/TimerManager.h"
#include "Engine/GameFrame/GameFrameHandler.h"

#include "Engine/Input/InputDataTypes.h"
#include "Engine/Input/InputActionValue.h"
#include "Engine/Input/InputAction.h"
#include "Engine/Input/InputMappingContext.h"
#include "Engine/Input/InputDeviceManager.h"
#include "Engine/Input/KeyboardDevice.h"
#include "Engine/Input/MouseDevice.h"

#include "Engine/LocalPlayer/LocalPlayerInputSystem.h"
#include "Engine/World/WorldContext.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/SceneActorMoveComponent.h"
#include "Engine/Component/EditorActorMoveComponent.h"
#include "Engine/Component/CameraComponent.h"

#include "Engine/Component/RenderComponent/RenderComponent.h"
#include "Engine/Component/RenderComponent/StaticSpriteComponent.h"
#include "Engine/Component/RenderComponent/DynamicSpriteComponent.h"

#include "Engine/Actor/Actor.h"
#include "Engine/Actor/CellActor.h"
#include "Engine/Actor/CameraActor.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneTracer.h"
#include "Engine/Scene/SceneRenderer.h"

#include "Engine/Resource/ResourceMananger.h"
#include "Engine/Scene/SceneManager.h"