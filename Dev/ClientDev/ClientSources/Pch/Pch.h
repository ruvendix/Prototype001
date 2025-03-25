// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// 컴파일된 ProtoBuffer는 공유 가능하지만 프로젝트마다 연결은 되어있어야 함!
#include "Network/Protobuf/Enum.pb.h"
#include "Network/Protobuf/Struct.pb.h"
#include "Network/Protobuf/ProtocolId.pb.h"
#include "Network/Protobuf/Packet.pb.h"

#include "EngineSources/UniversalInclusion.h"
#include "ServerCoreSources/UniversalInclusion.h"

// 서버와 클라이언트가 공용으로 사용합니다.
#include "Common/DataTypes.h"
#include "Common/Macros.h"

#pragma region 클라이언트만 사용하는 엔진 헤더 파일 모음입니다.
#include "EngineSources/Resource/ResourceMananger.h"
#include "EngineSources/GameFrame/GameFrameHandler.h"

#include "EngineSources/Input/InputDataTypes.h"
#include "EngineSources/Input/InputActionValue.h"
#include "EngineSources/Input/InputAction.h"
#include "EngineSources/Input/InputMappingContext.h"
#include "EngineSources/Input/InputDeviceManager.h"
#include "EngineSources/Input/KeyboardDevice.h"
#include "EngineSources/Input/MouseDevice.h"

#include "EngineSources/LocalPlayer/LocalPlayerInputSystem.h"

#include "EngineSources/Scene/Scene.h"
#include "EngineSources/Scene/SceneTracer.h"
#include "EngineSources/Scene/SceneRenderer.h"
#include "EngineSources/Scene/SceneManager.h"
#pragma endregion

#pragma region 클라이언트에서 자주 사용하는 헤더 파일 모음입니다.
#include "Global/DataTypes.h"
#include "ErrorHandler/ClientErrorHandler.h"
#include "GameEntity/GameEntityActor.h"
#include "Editor/EditorActor.h"

#include "Weapon/WeaponEnums.h"
#include "Weapon/WeaponActor.h"
#include "Weapon/Projectile/ProjectileStructs.h"
#include "Weapon/Projectile/ProjectileActor.h"
#include "Weapon/WeaponFactory.h"

#include "Player/PlayerActor.h"

#include "EnemyMonster/EnemyMonsterActor/EnemyMonsterActor.h"
#include "EnemyMonster/EnemyRespawner/EnemyRespawner.h"

#include "Network/NetworkManager.h"
#include "Network/Session/ServerSession.h"
#include "Network/Packet/ClientPacketHandler.h"
#pragma endregion

#ifdef _DEBUG
#pragma comment(lib, "EngineDev_Debug.lib")
#pragma comment(lib, "ServerCore_Debug.lib")
#pragma comment(lib, "libprotobufd.lib")
#else
#pragma comment(lib, "EngineDev.lib")
#pragma comment(lib, "ServerCore.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif