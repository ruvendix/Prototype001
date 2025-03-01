// Copyright 2025 Ruvendix, All Rights Reserved.
#pragma once

// 무시할 경고입니다.
#pragma warning(disable : 26495) // 변수가 초기화되지 않았다는 경고
#pragma warning(disable : 6011) // nullptr을 역참조했다는 경고

#pragma region 제공되는 헤더 파일입니다.
#include <crtdbg.h>
#include <cstdio>

#include <cassert>
#include <cmath>
#include <memory>
#include <mutex>
#include <bitset>
#include <vector>
#include <string>
#include <array>
#include <chrono>
#include <filesystem>
#include <functional>
#include <map>
#include <unordered_map>
#include <thread>
#include <format>
#include <queue>
#include <set>
#include <any>

#include <condition_variable>
#include <future>
#include <random>

using namespace std::chrono_literals;
#pragma endregion

#pragma region Windows 관련

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <timeapi.h>

// 네트워크 관련
#include <WinSock2.h>
#include <mswsock.h>
#include <WS2tcpip.h>

// lib 파일 링크입니다.
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma endregion