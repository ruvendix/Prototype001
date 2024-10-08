#pragma once

#include "Types.h"
#include "Windows.h"

enum class EInputState
{
	Up = 0, // 뗀 상태
	Press,  // 계속 누르고 있는 상태
	Down    // 처음에 누른 상태
};

enum class EInputValue
{
	Left     = VK_LEFT,
	Right    = VK_RIGHT,
	Up       = VK_UP,
	Down     = VK_DOWN,
	Q        = 'Q',
	E        = 'E',
	Esc      = VK_ESCAPE,
	LButton  = VK_LBUTTON,
	LCtrl    = VK_LCONTROL,
	Count    = 256
};