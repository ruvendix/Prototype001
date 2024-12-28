// Copyright 2024 Ruvendix, All Rights Reserved.
#pragma once

enum class EKeyboardValue
{
	Left = 0,
	Right,
	Down,
	Up,
	A,
	X,

	Count,
};

enum class EMouseValue
{
	LeftButton = 0,

	Count,
};

enum class EUserInputState
{
	Down = 0,
	Pressing,
	Up,

	Count,
};