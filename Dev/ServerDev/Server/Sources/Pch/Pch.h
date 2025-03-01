// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// �����ϵ� ProtoBuffer�� ���� ���������� ������Ʈ���� ������ �Ǿ��־�� ��!
#include "Protobuf/Enum.pb.h"
#include "Protobuf/Struct.pb.h"
#include "Protobuf/Protocol.pb.h"

#include "CommonInclusion.h"

#include "ServerCore/Sources/Global/Types.h"
#include "ServerCore/Sources/Thread/ThreadPool.h"

#include "ServerCore/Sources/Network/IocpCore.h"
#include "ServerCore/Sources/Network/IocpObject.h"
#include "ServerCore/Sources/Network/NetworkAddress.h"
#include "ServerCore/Sources/Network/Buffer/ReceiveBuffer.h"
#include "ServerCore/Sources/Network/Buffer/SendBuffer.h"
#include "ServerCore/Sources/Network/Session/PacketSession.h"
#include "ServerCore/Sources/Network/Service.h"

#include "ServerCore/Sources/Socket/SocketUtility.h"

#include "Global/Types.h"

#ifdef _DEBUG
#pragma comment(lib, "ServerCore_Debug.lib")
#pragma comment(lib, "libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore.lib")
#pragma comment(lib, "libprotobuf.lib")
#endif