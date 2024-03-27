/*
<���� ���>
�Է°� ���õ� ������ ����
*/

#pragma once

#include "Types.h"
#include "Windows.h"

enum class EInputState
{
	Up = 0, // �� ����
	Press,  // ��� ������ �ִ� ����
	Down    // ó���� ���� ����
};

enum class EInputValue
{
	Left     = VK_LEFT,
	Right    = VK_RIGHT,
	Up       = VK_UP,
	Down     = VK_DOWN,
	Esc      = VK_ESCAPE,
	LButton = VK_LBUTTON,
	Count = 256
};