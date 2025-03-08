// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// 컴파일된 ProtoBuffer는 공유 가능하지만 프로젝트마다 연결은 되어있어야 함!
#include "Protobuf/Enum.pb.h"
#include "Protobuf/Struct.pb.h"
#include "Protobuf/ProtocolId.pb.h"
#include "Protobuf/Packet.pb.h"

#include "EngineSources/UniversalInclusion.h"
#include "ServerCoreSources/UniversalInclusion.h"

// 서버와 클라이언트가 공용으로 사용합니다.
#include "Common/DataTypes.h"
#include "Common/Macros.h"

#include "Global/DataTypes.h"
#include "Packet/ServerPacketHandler.h"
#include "Session/GameSession.h"
#include "Session/GameSessionManager.h"

#include "Game/GameEntity.h"
#include "Game/GamePlayer.h"
#include "Game/GameRoom.h"

#ifdef _DEBUG
#pragma comment(lib, "EngineDev_Debug.lib")
#pragma comment(lib, "ServerCore_Debug.lib")
#pragma comment(lib, "libprotobufd.lib")
#else
#pragma comment(lib, "EngineDev.lib")
#pragma comment(lib, "ServerCore.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif