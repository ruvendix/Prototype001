// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// �����ϵ� ProtoBuffer�� ���� ���������� ������Ʈ���� ������ �Ǿ��־�� ��!
#include "Protobuf/Enum.pb.h"
#include "Protobuf/Struct.pb.h"
#include "Protobuf/Protocol.pb.h"

#include "CommonInclusion.h"

#include "ServerCore/Sources/ServerCoreGlobal/Inclusion.h"
#include "Global/Types.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore_Debug.lib")
#pragma comment(lib, "libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif