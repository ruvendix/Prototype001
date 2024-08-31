#pragma once

#pragma warning(disable: 6031)  // 함수 반환값 무시되었다는 경고
#pragma warning(disable: 26495) // 멤버 변수를 초기화하지 않았다는 경고
#pragma warning(disable: 28020) // 뭔지 모를 경고

#pragma comment(lib, "Msimg32.lib")

/*
SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
*/
#include <SDKDDKVer.h>

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define WIN32_LEAN_AND_MEAN 

// Windows 헤더 파일
#include <windows.h>

// 표준 헤더 파일입니다.
#include <crtdbg.h>
#include <cassert>
#include <type_traits>
#include <array>
#include <vector>
#include <format>
#include <memory>
#include <filesystem>
#include <unordered_map>

// 자주 사용하는 커스텀 헤더 파일입니다.
#include "Types.h"
#include "Defines.h"
#include "SharedPointers.h"
#include "GlobalHelper.h"
#include "Vec2d.h"