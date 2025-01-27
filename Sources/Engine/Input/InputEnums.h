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

enum class EInputValue
{
	Left = 0,
	Right,
	Down,
	Up,
	A,
	X,

	KeyboardEnd,
	MouseLeftButton,

	Count,
};

enum class EInputTrigger
{
	Pressed = 0,
	Holding,
	Released,

	Count,
};

enum class EInputActionValueType
{
	Boolean,
	Vector1,
	Vector2,
	Vector3
};