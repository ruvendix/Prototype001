// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

// 무시할 경고입니다.
#pragma warning(disable : 26495) // 변수가 초기화되지 않았다는 경고
#pragma warning(disable : 6011) // nullptr을 역참조했다는 경고

#pragma region 제공되는 헤더 파일입니다.
#include <crtdbg.h>
#include <cstdio>

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
#include <unordered_map>
#include <thread>
#include <format>
#include <queue>
#include <any>
#pragma endregion

/*
SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다. */
#include <SDKDDKVer.h>

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN 

// Windows 헤더 파일
#include <windows.h>
#include <timeapi.h>

// 엔진에서 자주 사용하는 헤더 파일 모음입니다.
#include "Engine/Global/Inclusion.h"

// 클라이언트에서 자주 사용하는 헤더 파일 모음입니다.
#include "Client/Global/Inclusion.h"

// lib 파일 링크입니다.
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Msimg32.lib")