// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

/*
이전 프레임에서는 누르지 않았지만 현재 프레임에서는 누른 경우와
이전 프레임에서 눌렀는데 현재 프레임에서도 누른 경우일 때
최상위 비트는 1이 되므로 0x8000으로 확인 가능
*/
#define IS_EXIST_LOCAL_PLAYER_INPUT(virtualKeyCode) ((::GetAsyncKeyState(virtualKeyCode) & 0x8000) != 0)