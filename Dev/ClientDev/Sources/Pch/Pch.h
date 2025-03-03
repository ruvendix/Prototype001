// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// 컴파일된 ProtoBuffer는 공유 가능하지만 프로젝트마다 연결은 되어있어야 함!
#include "ClientSources/Network/Protobuf/Enum.pb.h"
#include "ClientSources/Network/Protobuf/Struct.pb.h"
#include "ClientSources/Network/Protobuf/Protocol.pb.h"

#include "google/protobuf/message_lite.h"

#include "CommonInclusion.h"

// 서버에서 자주 사용하는 헤더 파일 모음입니다.
#include "ServerCore/Sources/ServerCoreGlobal/Inclusion.h"

// 엔진에서 자주 사용하는 헤더 파일 모음입니다.
#include "EngineSources/Global/Inclusion.h"

// 클라이언트에서 자주 사용하는 헤더 파일 모음입니다.
#include "ClientSources/Global/Inclusion.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore_Debug.lib")
#pragma comment(lib, "libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif