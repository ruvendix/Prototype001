// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// 여러 프로젝트에서 사용할 엔진 헤더 파일 모음입니다.
#include "EngineSources/UniversalInclusion.h"

#pragma region 엔진에서 많이 사용하는 헤더 파일들입니다.
#include "Input/InputDataTypes.h"
#include "Input/InputActionValue.h"
#include "Input/InputAction.h"
#include "Input/InputMappingContext.h"
#include "Input/InputDeviceManager.h"
#include "Input/KeyboardDevice.h"
#include "Input/MouseDevice.h"

#include "Component/ComponentDataTypes.h"
#include "Component/Component.h"
#include "Component/TransformComponent.h"
#include "Component/CellActorMoveComponent.h"
#include "Component/CameraComponent.h"
#include "Component/RenderComponent/RenderComponent.h"
#include "Component/RenderComponent/StaticSpriteComponent.h"
#include "Component/RenderComponent/DynamicSpriteComponent.h"

#include "Actor/ActorDataTypes.h"
#include "Actor/Actor.h"
#include "Actor/CameraActor.h"
#include "Actor/CellActor.h"
#include "Actor/EffectActor/EffectActor.h"
#include "Actor/PawnActor/PawnActor.h"

#include "Scene/Scene.h"
#include "Scene/SceneTracer.h"
#include "Scene/SceneRenderer.h"
#include "Scene/SceneManager.h"

#include "World/WorldContext.h"
#pragma endregion